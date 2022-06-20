//Ricardo Arriaga
//A01570553

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

//Complejidad: O(n)
vector<int> LPS(string patron)
{
    int n = patron.length();
    vector<int> lps(n, 0);
    int j = 0;
    int i = 1;

    while(i < n)
    {
        if(patron[i] == patron[j])
        {
            lps[i] = j + 1;
            j++;
            i++;
        }
        else
        {
            if(j == 0)
            {
                lps[i] = 0;
                i++;
            }
            else
            {
                j = lps[j - 1];
            }
        }
    }
    return lps;
}

//Complejidad: O(n)
void findMCodes(string mCode, string transmission, int tNum, ofstream& cFile)
{
    vector<int> mCPositions;
    vector<int> lps = LPS(mCode);
    int mcIdx = 0;
    int tIdx = 0;

    while(tIdx < transmission.size())
    {
        if(transmission[tIdx] == mCode[mcIdx])
        {
            tIdx++;
            mcIdx++;

            if(mcIdx == mCode.size())
            {
                mCPositions.push_back(tIdx - mCode.size());
                mcIdx = lps[mcIdx-1];
            }
        }
        else
        {
            if(mcIdx == 0)
            {
                tIdx++;
            }
            else
            {
                mcIdx = lps[mcIdx - 1];
            }
        }
    }
    
    cFile << "Transmission" << tNum << ".txt ==> " << mCPositions.size() << " veces" << "\n";
    for(int i = 0; i < mCPositions.size(); i++)
    {
        if(i != mCPositions.size() - 1)
        {
            cFile << mCPositions[i] << ", ";
        }
        else
        {
            cFile << mCPositions[i] << "\n";
        }
    }
    cFile << "\n";
}

//Complejidad: O(n)
void findLongestPalindrome(string transmission, ofstream& cFile, int tNum)
{
    int tSize = transmission.size();
    int centerIdx = 1;
    int rightIdx = 2;
    int right;
    int left;
    int start;
    int end;
    int diff;
    int maxPalLenght = 0;
    int maxCenterIdx = 0;

    tSize = (tSize * 2) + 1;
    vector<int> pal(tSize);
    pal[0] = 0;
    pal[1] = 1;
    
    for(right = 2; right < tSize; right++)
    {
        left = (centerIdx * 2) - right;
        pal[right] = 0;
        diff = rightIdx - right;

        if(diff > 0)
        {
            pal[right] = min(pal[left], diff);
        }

        while((right + pal[right]) < tSize && (right - pal[right]) > 0 && (((right + pal[right] + 1) % 2 == 0) || 
        (transmission[(right + pal[right] + 1) / 2] == transmission[(right - pal[right] - 1) / 2])))
        {
            pal[right]++;
        }
        
        if(pal[right] > maxPalLenght)
        {
            maxPalLenght = pal[right];
            maxCenterIdx = right;
        }

        if(right + pal[right] > rightIdx)
        {
            centerIdx = right;
            rightIdx = right + pal[right];
        }
    }

    start = (maxCenterIdx - maxPalLenght) / 2;
    end = start + maxPalLenght - 1;
    string palindrome;

    for(int i = start; i <= end; i++)
    {
        palindrome += transmission[i];
    }

    cFile << "Transmission" << tNum << ".txt ==> Posicion: " << start << "\n";
    cFile << palindrome << "\n";
    cFile << "--------------------------------------" << "\n";
}

//Complejidad: O(n*m)
void findLCS(string transmissionA, string transmissionB, ofstream& cFile)
{
    int tAl = transmissionA.length();
    int tBl = transmissionB.length();
    int max = 0;
    int maxColumn;
    int subStrIdx;
    vector<vector<int>> lcs(tAl, vector<int>(tBl));

    for(int i = 0; i < tAl; i++)
    {
        if(transmissionA[i] == transmissionB[0])
        {
            lcs[i][0] = 1;
            max = 1;
        }
        else
        {
            lcs[i][0] = 0;
        }
    }

    for(int i = 0; i < tBl; i++)
    {
        if(transmissionB[i] == transmissionA[0])
        {
            lcs[0][i] = 1;
            max = 1;
        }
        else
        {
            lcs[0][i] = 0;
        }
    }

    for(int i = 1; i < tAl; i++)
    {
        for(int j = 1; j < tBl; j++)
        {
            if(transmissionA[i] == transmissionB[j])
            {
                lcs[i][j] = lcs[i-1][j-1] + 1;
                
                if(lcs[i][j] > max)
                {
                    max = lcs[i][j];
                    maxColumn = j;
                }
            }
            else
            {
                lcs[i][j] = 0;
            }
        }
    }

    subStrIdx = (maxColumn - max) + 1;

    for(int i = 0; i < max; i++)
    {
        cFile << transmissionB[subStrIdx];
        subStrIdx++;
    }

    lcs.clear();
}

int main()
{
    string line;
    string t1;
    string t2;
    string t3;
    string mc;
    vector<string> transmissions;
    vector<string> mCodes;

    ifstream file1("transmission1.txt");
    ifstream file2("transmission2.txt");
    ifstream file3("transmission3.txt");
    ifstream mcfile("mcode.txt");

    while (getline(file1, line))
    {
        t1 += line;
    }

    file1.close();
    transmissions.push_back(t1);

    while (getline(file2, line))
    {
        t2 += line;
    }

    file2.close();
    transmissions.push_back(t2);

    while (getline(file3, line))
    {
        t3 += line;
    }

    file3.close();
    transmissions.push_back(t3);

    while (getline(mcfile, mc))
    {
        mCodes.push_back(mc);
    }

    mcfile.close();

    ofstream checkFile("checking.txt");

    for(int i = 0; i < mCodes.size(); i++)
    {
        checkFile << "Codigo: " << mCodes[i] << "\n";
        checkFile << "\n";
        for(int j = 0; j < transmissions.size(); j++)
        {
            findMCodes(mCodes[i], transmissions[j], j+1, checkFile);
        }
        checkFile << "--------------------------------------" << "\n";
    }

    checkFile << "======================================" << "\n";
    
    checkFile << "Palíndromo más grande:" << "\n";

    for(int i = 0; i < transmissions.size(); i++)
    {
        findLongestPalindrome(transmissions[i], checkFile, i+1);
    }
    
    checkFile << "======================================" << "\n";

    checkFile << "Substring más largo de T1 y T2:" << "\n";

    findLCS(transmissions[0], transmissions[1], checkFile);
    checkFile << "\n";
    checkFile << "--------------------------------------" << "\n";
    
    checkFile << "Substring más largo de T1 y T3:" << "\n";

    findLCS(transmissions[0], transmissions[2], checkFile);
    checkFile << "\n";
    checkFile << "--------------------------------------" << "\n";
    
    checkFile << "Substring más largo de T2 y T3:" << "\n";

    findLCS(transmissions[1], transmissions[2], checkFile);
    checkFile << "\n";
    checkFile << "--------------------------------------";

    return 0;
}