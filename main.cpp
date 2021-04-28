#include <iostream>
#include<fstream>
#include <cmath>

using namespace std;
// a point signifies (x,y) in a 2d plane
class point
    {
        public:
        double xcoord;
        double ycoord;
        int label;
        double distance;
    };

class kMean
{ 
    public:
    int k;
    int numPts;
    point* pointset;
    int numRows;
    int numCols;
    int** displayAry;
    point* kCentroidAry;
    int changeFlag = 0;
    int iteration = 0;

    // pointset is an array of points
    //values loaded from txt file
    void loadPointSet(ifstream& inFile, point *pointset)
    {
        int index = 0;
        double x;
        double y;
        while(inFile >> x >> y)
        {
            pointset[index].xcoord = x;
            pointset[index].ycoord = y;
            pointset[index].label = 0;
            pointset[index].distance = 99999.00;
            index++;
        }
        

    }

    //display for debugging purposes
    void plotDisplayArray()
    {
        for(int i=0; i < numPts; i++)
        {
            displayAry[(int)pointset[i].xcoord][(int)pointset[i].ycoord] = pointset[i].label;
        }
        cout <<"iteration: " << iteration<<" change: " << changeFlag << endl;
        for(int i =0; i < numRows; i++ )
       {
         for(int j = 0; j < numCols; j++)
        {
        if( displayAry[i][j]> 0)
          cout << displayAry[i][j];
          else
          {
              cout <<" ";
          }
          
        }
        cout << endl;
       }
       cout<< "yass";
    }
    
    //calculate distance of each point from k centroids(defined below) to make k clusters of points that are relatively 
    //closer.
    //repeated calculations of centroid and min label creates clusters that are definite.
    
    int distanceMinLabel(point pt,double & minDist)
    {
        minDist = 99999.00;
        int minLabel = 0;
        int label = 1;
        while(label <= k)
        {
        point whichCentroid = kCentroidAry[label];
        double distance = sqrt(pow((pt.xcoord - whichCentroid.xcoord),2)+pow((pt.ycoord - whichCentroid.ycoord),2));
        
        if (distance < minDist)
        {
            minDist = distance;
            minLabel = label;
        }
        label++;
        }
        return minLabel;
        
    }

    void selectKCentroids ()
    {
       
        int kCnt = 0;
        int Aryindexes[k];
        int index;
        while(kCnt < k)
        { 
            index = rand()%numPts-1;
            cout << "index: "<<  index<<" " ;
            Aryindexes[kCnt] = index;
            while (repeat(Aryindexes, kCnt) == false)
            {
                index = rand()%numPts-1;
                Aryindexes[kCnt] = index;

            }
            
            kCentroidAry[kCnt].xcoord = pointset[index].xcoord;
            kCentroidAry[kCnt].ycoord = pointset[index].ycoord;
            kCentroidAry[kCnt].label = kCnt+1;
            pointset[index].label = kCnt+1;
            cout <<"ps"<< pointset[index].label ;
            kCentroidAry[kCnt].distance = 0.00;
            kCnt++;
        }
        
         
    }

    bool repeat(int* Ary, int kCnt)
    {
        
        
            for (int i = 0; i < kCnt-1; i++)
            {
            
             if(Ary[kCnt]== Ary[i]) 
            return false;
            }
            return true;
        
    }

    void computeCentroids()
    { 
       
        double sumX[k+1];
        double sumY[k+1];
        int totalPt[k+1];
        int index = 0;
        int label = 1;
        cout <<"computing all centroids" <<endl;
        while(index < numPts)
        {
        label = pointset[index].label;
        sumX[label]+=pointset[index].xcoord;
        sumY[label]+=pointset[index].ycoord;
        totalPt[label]++;
        index++;
        }
        cout << sumX[1] <<sumY[2] <<totalPt[2] <<endl;
        while(label < k)
        {
        if(totalPt[label] > 0)
        {
            if (kCentroidAry[label].xcoord == sumX[label]/totalPt[label] && kCentroidAry[label].ycoord == (sumY[label]/totalPt[label]))
            {
                changeFlag = 0;
                cout <<" no change"<<endl;
                
            } 
            else
            {
                cout <<"changing label: " <<label<<" ";
                changeFlag++;
                kCentroidAry[label].xcoord = (sumX[label]/totalPt[label]);
                kCentroidAry[label].ycoord = (sumY[label]/totalPt[label]);
            }
            cout << "changeFlag: "<<changeFlag <<" "<< endl;
            
            }
            label++;
        }

    }
    
    
    void kMeansClustering(point* pointset, int k, point* kCentroidArray, ofstream &outfile1, ofstream &outfile2 )
    {
        for (int i = 0; i < 3;i++)
        {
        selectKCentroids();
        plotDisplayArray(); 
        prettyPrint(outfile1);
        int index = 0;
        while(index <= numPts)
        { 
            point pt = pointset[index];
            double minDist = pointset[index].distance;
            int minLabel = distanceMinLabel(pt, minDist);
            if(pointset[index].label!= minLabel)
            {
                pointset[index].label = minLabel;
                pointset[index].distance = minDist; 
            }
            index++;
        }
        changeFlag++;
        plotDisplayArray(); 
        computeCentroids();
        iteration++;
        }
        //works but loops eternally using a constraint up to show output
       /* while (changeFlag != 0)
        {
           int index = 0;
           iteration++;
           plotDisplayArray(); 
           int changes;
           while(index <= numPts)
           { 
            changes = 0;
            point pt = pointset[index];
            double minDist = pointset[index].distance;
            int minLabel = distanceMinLabel(pt, minDist);
            if(pointset[index].label!= minLabel)
            {
                pointset[index].label = minLabel;
                pointset[index].distance = minDist; 
                changes++;
            }
            index++;
           }
           if (changes > 2)
           {
            computeCentroids();
            changeFlag = 0;
           }
        }*/
        
        
    } 
     void prettyPrint (ofstream &outFile1)
     {
        outFile1 <<"iteration: " << iteration<<" kVal: " << k << endl;
        for(int i =0; i < numRows; i++ )
        {
            for(int j = 0; j < numCols; j++)
            {
            if( displayAry[i][j]> 0)
                outFile1 << displayAry[i][j];
            else
            {
              outFile1 <<" ";
            }  
        }
        outFile1 << endl;
       }

     }  
     void printPointRestult(ofstream &outfile2) 
     {
         outfile2 <<numRows <<" "<< numCols << endl;
         outfile2<<numPts << endl;
         for (int i = 0; i < numPts; i++)
         {
             outfile2 << pointset[i].xcoord <<" " <<pointset[i].ycoord <<" " <<pointset[i].label<<endl;
         }
     }
        
    
};

int main(int argc,char** argv)
{
    //open files
    ifstream inFile;
    inFile.open(argv[1]);
    ofstream outFile1;
    outFile1.open(argv[3]);
    ofstream outFile2;
    outFile2.open(argv[4]);
    //read initial val
    int numRows, numCols, numPts;
    inFile >> numRows >> numCols >> numPts;
    //kmean objects
    kMean* myk = new kMean();
    myk->k = atoi(argv[2]);
    myk -> numRows = numRows;
    myk -> numCols = numCols;
    myk -> numPts = numPts;
    myk ->pointset = new point[myk->numPts];
    myk ->kCentroidAry = new point[myk->k];
    myk->loadPointSet (inFile,myk->pointset);
    myk-> displayAry = new int* [myk->numRows];
    for (int i = 0; i < myk->numRows; i++)
    {
        myk->displayAry[i] = new int[myk->numCols];
    }
    for(int i =0; i < myk->numRows; i++ )
    {
    for(int j = 0; j < myk->numCols; j++)
    {
          myk ->displayAry[i][j] = 0;
    }
    }
    
    myk -> kMeansClustering(myk->pointset, myk->k, myk ->kCentroidAry, outFile1, outFile2);
    myk->printPointRestult(outFile2);
    inFile.close();
    outFile1.close();
    outFile2.close();

   /* for (int i = 0; i < k; i++)
    {
        cout << myk->kCentroidAry[i].xcoord <<" " <<myk->kCentroidAry[i].ycoord <<" "<< myk->kCentroidAry[i].label << endl;
    }*/




}