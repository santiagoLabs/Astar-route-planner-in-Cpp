#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;
using std::cout;
using std::cin;

// Read file using the file passed as parameter
static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
  	// Inizialization of file stream object called is 
  	// binary means it will read the data as binary data
  	// ate (a the end) means it will immediately seek the end of the file
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
  	// Determine the size of the input string
    auto size = is.tellg();
  	// Initialize vector of bytes with same size as input file 
    std::vector<std::byte> contents(size);    
    
  	// Return at the beginning of the input file
    is.seekg(0);
   	// Run through the file and store into the vector
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
  	// return the vector
  	// Move transfers the content of the vector without using references or pointers 
    return std::move(contents);
}

int main(int argc, const char **argv)
{    
  	float start_x, start_y, end_x, end_y;
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
	
  	cout<<"Insert starting coordinates x y:";
  	cin>>start_x>>start_y;
  	cout<<"Insert ending coordinated x y:";
  	cin>>end_x>>end_y;
  
    // Build Model.
    RouteModel model{osm_data};

    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
