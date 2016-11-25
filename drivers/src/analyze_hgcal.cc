#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH1D.h"
#include "TF1.h"

int main(int argc, char** argv)
{
    //  chain in all the input files
    std::string filename = argv[1];
    std::ifstream input(filename);
    TChain *chain = new TChain("Events");
    for (std::string line; std::getline(input, line);)
        chain->Add(TString(line.c_str()));

    // branching
    double response[30][30][30];
    double energy;
    chain->SetBranchAddress("emresponseImage", response);
    chain->SetBranchAddress("energy", &energy);

    // output declaration/defintion
    TFile *out = new TFile("test_analyze.root", "RECREATE");
    int nEnergies = 8;
    int energiesConsidered[] = {1, 2, 4, 8, 16, 32, 50, 60};
    std::map<int, TProfile2D*> pImageProfiles;
    std::map<int, TH1D*> hResponses;
    std::map<int, TH1D*> hEnergies;
    std::map<int, TProfile*> pPerLayerResponses;

    TGraph *gResponseVsEnergy = new TGraph();
    TGraph *gEnergyResolution = new TGraph();
    gEnergyResolution->SetName("EnergyResolution");
    gResponseVsEnergy->SetName("ResponseVsEnergy");

    char histname[200];
    for (int i=0; i<nEnergies; i++)
    {
        sprintf(histname, "Response__%dGeV", energiesConsidered[i]);
        hResponses[energiesConsidered[i]] = new TH1D(histname, histname, 10000,
            energiesConsidered[i]*1000000, energiesConsidered[i]*10000000);
        hResponses[energiesConsidered[i]]->GetXaxis()->SetTitle("Response");
        hResponses[energiesConsidered[i]]->GetYaxis()->SetTitle("Entries");

        sprintf(histname, "ImageProfile__%dGeV", energiesConsidered[i]);
        pImageProfiles[energiesConsidered[i]] = new TProfile2D(histname, histname,
            30, 0, 30, 30,0,30, 0, 10000000);
        pImageProfiles[energiesConsidered[i]]->GetXaxis()->SetTitle("cm");
        pImageProfiles[energiesConsidered[i]]->GetYaxis()->SetTitle("cm");
        pImageProfiles[energiesConsidered[i]]->GetZaxis()->SetTitle("Response");

        sprintf(histname, "PerLayerResponse__%dGeV", energiesConsidered[i]);
        pPerLayerResponses[energiesConsidered[i]] = new TProfile(histname, histname,
            30, 0, 30, 0, 100000000);
        pPerLayerResponses[energiesConsidered[i]]->GetXaxis()->SetTitle("Layer");
        pPerLayerResponses[energiesConsidered[i]]->GetYaxis()->SetTitle("Response");

        sprintf(histname, "Energy__%dGeV", energiesConsidered[i]);
        hEnergies[energiesConsidered[i]] = new TH1D(histname, histname, 1000,
            energiesConsidered[i]-0.5*energiesConsidered[i],
            energiesConsidered[i]+0.5*energiesConsidered[i]);
        hEnergies[energiesConsidered[i]]->GetXaxis()->SetTitle("Energy (GeV)");
        hEnergies[energiesConsidered[i]]->GetYaxis()->SetTitle("Entries");
    }

    int n = chain->GetEntries();
    for (int ievent=0; ievent<n; ievent++)
    {
        chain->GetEntry(ievent);
        if (ievent%100==0) 
            std::cout << "Event " << ievent << " / " << n  << " Energy=" << energy
            << std::endl;

        double totalYield = 0;
        double totalYieldNoWeights = 0;
        for (int ilayer=0; ilayer<30; ilayer++)
        {
            double yield = 0;
            for (int i=0; i<30; i++)
                for (int j=0; j<30; j++)
                {
                    yield += response[ilayer][i][j];
                    totalYieldNoWeights += response[ilayer][i][j];
                    if (ilayer<10) totalYield += response[ilayer][i][j];
                    else if (ilayer<20) totalYield += 2*response[ilayer][i][j];
                    else totalYield += 3*response[ilayer][i][j];
                    pImageProfiles[(int)energy]->Fill(j, i, (ilayer/10 + 1)*response[ilayer][i][j]);
                }
            pPerLayerResponses[(int)energy]->Fill(ilayer, yield);
        }
        hResponses[(int)energy]->Fill(totalYield);
    }

    for (int ie=0; ie<nEnergies; ie++)
    {
        hResponses[energiesConsidered[ie]]->Rebin(50);

        // fit the response
        Double_t min = hResponses[energiesConsidered[ie]]->GetMean() - 
            1*hResponses[energiesConsidered[ie]]->GetRMS();
        Double_t max = hResponses[energiesConsidered[ie]]->GetMean() + 
            2*hResponses[energiesConsidered[ie]]->GetRMS();
        TF1 *myGaus = new TF1("myGaus", "gaus", min, max);
        hResponses[energiesConsidered[ie]]->Fit("myGaus", "R");
        TF1 *fit = hResponses[energiesConsidered[ie]]->GetFunction("myGaus");
        gResponseVsEnergy->SetPoint(ie, energiesConsidered[ie], 
            fit->GetParameter(1));
    }

    // aux + fit
    gResponseVsEnergy->SetMarkerStyle(20);
    TF1 *flin = new TF1("flin", "[0]*x", energiesConsidered[0], energiesConsidered[nEnergies-1]);
    gResponseVsEnergy->Fit("flin", "R");

    // energy reco
    double calibrationCoefficient = gResponseVsEnergy->GetFunction("flin")->GetParameter(0);
    std::cout << "calibrationCoefficient = " << calibrationCoefficient << std::endl;
    for (int ievent=0; ievent<n; ievent++)
    {
        chain->GetEntry(ievent);
        if (ievent%100==0) 
            std::cout << "Event " << ievent << " / " << n  << " Energy=" << energy
            << std::endl;

        double totalYield = 0;
        double totalYieldNoWeights = 0;
        for (int ilayer=0; ilayer<30; ilayer++)
        {
            double yield = 0;
            for (int i=0; i<30; i++)
                for (int j=0; j<30; j++)
                {
                    yield += response[ilayer][i][j];
                    totalYieldNoWeights += response[ilayer][i][j];
                    if (ilayer<10) totalYield += response[ilayer][i][j];
                    else if (ilayer<20) totalYield += 2*response[ilayer][i][j];
                    else totalYield += 3*response[ilayer][i][j];
                }
        }
        hEnergies[(int)energy]->Fill(totalYield/calibrationCoefficient);
    }
    
    for (int ie=0; ie<nEnergies; ie++)
    {
        hEnergies[energiesConsidered[ie]]->Rebin(10);

        Double_t min = hEnergies[energiesConsidered[ie]]->GetMean() - 
            1*hEnergies[energiesConsidered[ie]]->GetRMS();
        Double_t max = hEnergies[energiesConsidered[ie]]->GetMean() + 
            1*hEnergies[energiesConsidered[ie]]->GetRMS();
        TF1 *myGaus = new TF1("myGaus", "gaus", min, max);
        hEnergies[energiesConsidered[ie]]->Fit("myGaus", "R");
        TF1 *fit = hEnergies[energiesConsidered[ie]]->GetFunction("myGaus");
        //  
//        gEnergyResolution->SetPoint(ie, energiesConsidered[ie], 
//            fit->GetParameter(2)/fit->GetParameter(1));
        gEnergyResolution->SetPoint(ie, energiesConsidered[ie],
            hEnergies[energiesConsidered[ie]]->GetRMS()/
            hEnergies[energiesConsidered[ie]]->GetMean());
        gEnergyResolution->SetMarkerStyle(22);
    }

    TF1* fres = new TF1("fres", "sqrt(pow([0]/sqrt(x),2) + pow([1],2))",
        energiesConsidered[0], energiesConsidered[7]);
    fres->SetParName(0, "#alpha");
    fres->SetParName(1, "C");
    gEnergyResolution->Fit("fres", "R");
    
    // write
    gEnergyResolution->GetXaxis()->SetTitle("Energy (GeV)");
    gEnergyResolution->GetYaxis()->SetTitle("#sigma / E");
    gResponseVsEnergy->GetXaxis()->SetTitle("Energy (GeV)");
    gResponseVsEnergy->GetYaxis()->SetTitle("Response");
    gResponseVsEnergy->Write();
    gEnergyResolution->Write();
    out->Write();
    out->Close();

    return 0;
}
