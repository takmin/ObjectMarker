#include "ObjectMarker.h"


int main(int argc, char* argv[])
{
	std::string config_file = (argc > 1) ? argv[1] : "config.xml";
	ObjectMarker object_marker;

	object_marker.run(config_file);
}
