

#include <vector>
#include "AShape.h"
#include <cstring>

class PathIO
{
public:
	PathIO();
	~PathIO();

	// save to a file
	void SavePath(std::vector<AShape> paths, std::string filename);
	
	// load from a file
	std::vector<AShape> LoadPath(std::string filename);

};
