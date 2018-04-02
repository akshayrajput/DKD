#include"allstdlib.h"
#include"Map.h"

extern MapNode MAP[MAPW][MAPH];

int _manhattanDistance(int sx, int sy, int dx, int dy)
{
	return (abs(sx - dx) + abs(sy - dy));
}

vector<MOVEMENT> direction(int sx, int sy, int dx, int dy)
{
	int fi, fj, fif, fjf,g_temp,h_temp;
	std::vector<MOVEMENT> direc;
	std::vector<std::pair<int, int> > closed;
	int  change = 1;
	int  min = INT_MAX;

	if(MAP[dx][dy].getType() == OBS)
	{
		cout<<"Its an obstacle!!. MapX- "<<dx<<" MapY - "<<dy<<"\n";
		return direc;
	}

	while(change != 0)
	{
		closed.push_back(make_pair(sx, sy));
		if(sx == dx && sy == dy)
			break;

		change = 0;
		min = INT_MAX;
		for(int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <=1; j++)
			{
			
				fi = sx + i;
				fj = sy + j;
				
				
				// New cordinates should be inside boundary and should not be an obstruction

				if(fi >= 0 && fi <= 24 && fj >= 0 && fj <= 24 && MAP[fi][fj].getType() != OBS && std::find(closed.begin(),closed.end(),make_pair(fi,fj))==closed.end())
				{
					g_temp = (abs(fi - sx) + abs(fj - sy));
					h_temp = _manhattanDistance(fi, fj, dx, dy);
					if(min > h_temp)//if(min > (g_temp + h_temp))
					{
						change = 1;
						min = h_temp;//min = g_temp + h_temp;
						fif = fi;
						fjf = fj;
					}
				}
			}
		}

		if(change)
		{
			if(sx > fif && sy > fjf)
				direc.push_back(NW);
			else if(sx > fif && sy == fjf)
				direc.push_back(N);
			else if(sx > fif && sy < fjf)
				direc.push_back(NE);
			else if(sx == fif && sy > fjf)
				direc.push_back(W);
			else if(sx == fif && sy < fjf)
				direc.push_back(E);
			else if(sx < fif && sy > fjf)
				direc.push_back(SW);
			else if(sx < fif && sy == fjf)
				direc.push_back(S);
			else
				direc.push_back(SE);
		
		}

		sx = fif; sy = fjf;
		
	}
	return direc;
}


