#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>
using namespace std;

class Position 
{
public:
	Position(int _x, int _y) : x(_x), y(_y) {} 
	bool operator<(const Position& rhs) const
	{
		if(x != rhs.x)
			return x < rhs.x;
		return y < rhs.y;
	}
	
public:
	int x;
	int y;
};
enum EditOperation{
	INSERT,
	SUBSTITUTE,
	DELETE,
	NONE
};
class LevenshteinEditDistance{

public:
	void compute(vector<string> stbl, vector<string>  ttbl){
		vector<string>::iterator it;
		it = stbl.begin();
		stbl.insert(it,"\u0000");
		it = ttbl.begin();
		ttbl.insert(it,"\u0000");


		int n = stbl.size();
		int m = ttbl.size();
		

		int d[m+1][n+1]; // check
		std::map<Position, Position> parentMap;
		
		for(int i = 1; i <= m; ++i)
			d[i][0] = i;
		
		for(int j = 1; j <= n; ++j)
			d[0][j] = j;

		for(int j = 1; j <= n; ++j){
			for(int i = 1; i <= m; ++i){
				int delta;
				if(stbl[j-1].compare(ttbl[i-1])==0)
					delta = 0;
				else
					delta = 1;

				int tentativeDistance = d[i - 1][j] + 1;
				EditOperation editOperation = INSERT;

		                if (tentativeDistance > d[i][j - 1] + 1) {
                		    tentativeDistance = d[i][j - 1] + 1;
		                    editOperation = DELETE;
               			 }

		                if (tentativeDistance > d[i - 1][j - 1] + delta) {
                		    tentativeDistance = d[i - 1][j - 1] + delta;
			            editOperation = SUBSTITUTE;
                		}
		                d[i][j] = tentativeDistance;

				switch (editOperation) {
                 		   case SUBSTITUTE:
			                parentMap.insert(make_pair(Position(i,j),Position(i-1,j-1)));
			                break;

		                    case INSERT:
                		        parentMap.insert(make_pair(Position(i,j), Position(i-1,j)));
			                break;

		                    case DELETE:
 	             			parentMap.insert(make_pair(Position(i,j), Position(i, j-1)));
		                        break;
                		}
			}
		}
		GAP = "-";
		Position current(m,n);
		
		while(true){
			std::map<Position,Position>::iterator it = parentMap.find(current);

			if(it==parentMap.end())
				break;
			if(current.x != it->second.x && current.y != it->second.y){
				string a_str = stbl[it->second.y];
				string b_str = ttbl[it->second.x];


				topAlignmentRowVec.push_back(stbl[it->second.y]);
				bottomAlignmentRowVec.push_back(ttbl[it->second.x]);
				if(a_str.compare(b_str)==0)
					editSequenceVec.push_back(NONE);
				else	
					editSequenceVec.push_back(SUBSTITUTE);
			}else if(current.x != it->second.x) {
				topAlignmentRowVec.push_back(GAP);
				bottomAlignmentRowVec.push_back(ttbl[it->second.x]);
				editSequenceVec.push_back(INSERT);	
			}else{
				topAlignmentRowVec.push_back(stbl[it->second.y]);
				bottomAlignmentRowVec.push_back(GAP);
				editSequenceVec.push_back(DELETE);
			}
			current = it->second;
		}
	
		vector<EditOperation>::iterator  ita=editSequenceVec.end();
		ita--;
		editSequenceVec.erase(ita);
		vector<string>::iterator itb=topAlignmentRowVec.end();
		itb--;
		topAlignmentRowVec.erase(itb);
		itb = bottomAlignmentRowVec.end();
		itb--;
		bottomAlignmentRowVec.erase(itb);

		for(int i = topAlignmentRowVec.size()-1; i >= 0 ; i--)
			printf("%s\t",topAlignmentRowVec[i].c_str());
		printf("\n");
		for(int i = editSequenceVec.size()-1; i >= 0; i--)
			printf("%d\t",editSequenceVec[i]);
		printf("\n");
		for(int i = bottomAlignmentRowVec.size()-1; i >= 0; i--)
			printf("%s\t",bottomAlignmentRowVec[i].c_str());
		printf("\n");

	}


private:
	string GAP;
	vector<string> topAlignmentRowVec;
	vector<string> bottomAlignmentRowVec;
	vector<EditOperation> editSequenceVec;

};
// char 배열의 한글을 string type의 벡터로
int split(const char* pstr, vector<string>& tbl)
{
        int count=0;
        string s;
        char tmpstr[3];
	char buff[3];
        int len = strlen(pstr);
        for(int i = 0; i < len;){
                if(*pstr & 0x80){
			memcpy(tmpstr,pstr,3);
			sprintf(buff,"%c%c%c",tmpstr[0],tmpstr[1],tmpstr[2]);
			s = buff;
                        tbl.push_back(s);
                        pstr+=3;
                        i+=3;
                        count++;
                }else{
                        pstr++;
                        i++;
                }
        }
        return count;
}

int main()
{
	const char* psrc="나는사랑합니다";
	const char* ptrg="너는사당합다";

	vector<string> stbl;
	vector<string> ttbl;

	split(psrc,stbl);
	split(ptrg,ttbl);
	
	
	
	LevenshteinEditDistance led;
	led.compute(stbl,ttbl);
	return 1;

}
