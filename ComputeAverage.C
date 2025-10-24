
//int RunNumber[]={297678,297698,297710,297729,297828,297842,297882,297935,297969,297978,297989,298001,298010,298017,298060,298068};
int RunNumber[] = {298190, 298289, 298225, 298076, 298219, 298297, 298270, 298125, 298099, 298183, 298110, 298232, 298248, 298210, 298282, 298167, 298078, 298095, 298260, 298301};
  
const int nRuns = sizeof(RunNumber)/sizeof(RunNumber[0]);
vector<float> bkgProb[nRuns];
void ComputeAverage(){
  for(int i=0; i<nRuns; i++){
    TString fname;
    //fname.Form("./maps_files_test/back-para-histsum-%d-0-8.new-vector74-root",RunNumber[i]);
    fname.Form("./back-para-histsum-%d-0-8.new-vector74-root",RunNumber[i]);
    printf("%s\n",fname.Data());
    ifstream afile;
    afile.open(fname.Data());
    if(!afile) printf("File Open Error\n");
    float prob;
    int j=0;
    while(afile>>prob){
      //printf("%0.12f\n",prob);
      //j++;
      //if(j==10) continue;
      bkgProb[i].push_back(prob);
    }//while
  }//i
  //Make average
  int nPoints = bkgProb[0].size();
  std::vector<double> avg(nPoints, 0.0);
  for(int i=0; i<nRuns; i++){
    if(bkgProb[i].size()!=nPoints){
      printf("Run number %d has point missmatch\n",RunNumber[i]);
      break;
    }
    else{
      for(int j=0; j<=bkgProb[i].size(); j++){
	avg[j] += bkgProb[i][j];
      }//for npoints
    }//else check if all vectors have same points
  }//for all files
  for (int j = 0; j < nPoints; ++j) {
        avg[j] /= nRuns;
  }
  ofstream outmap;
  outmap.open("W09-back-para-histsum-average-0-8.new-vector74-root");
  for (int j = 0; j < nPoints; ++j) {
    outmap<<avg[j]<<" ";
  }
  outmap.close();
}
