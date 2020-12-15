#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
	  RoutePlanner::start_node = &m_Model.FindClosestNode(start_x, start_y);
  	RoutePlanner::end_node = &m_Model.FindClosestNode(end_x, end_y);
}



// Use the distance to the end_node for the h value.
float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance((*end_node));
}


// Expand the current node by adding all unvisited neighbors to the open list.
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    // Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
	  current_node -> FindNeighbors();
    //  For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
    //  Use CalculateHValue below to implement the h-Value calculation.
    //  For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.
  	for(auto *node : current_node->neighbors){
        node->parent = current_node;
      	node->.g_value = current_node->distance((*node)) + (*current_node).g_value;
      	node->.h_value = CalculateHValue(node);
      	RoutePlanner::open_list.push_back(node);
      	node->.visited = true;
    }
}


// Sort the open list and return the next node according to the sum of the h value and g value.
RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(RoutePlanner::open_list.begin(), RoutePlanner::open_list.end(), [](auto* n_1, auto* n_2){
      return n_1->h_value + f2 = n_2->h_value + n_2->g_value;
    });
  
  	RouteModel::Node* lowest_node = open_list.back();
  	open_list.pop_back();
  	return lowest_node;
}


// Return the final path found from A* search.
//  Take the current (final) node as an argument and iteratively follow the 
//  chain of parents of nodes until the starting node is found.
//  For each node in the chain, add the distance from the node to its parent to the distance variable.
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    
    while(current_node->parent != nullptr){
        distance += current_node->distance(*(current_node->parent));
      	path_found.push_back(*current_node);
        current_node = current_node->parent;
    }
  	path_found.push_back(*current_node);
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
  	std::reverse(path_found.begin(), path_found.end());
    return path_found;

}


//  The AddNeighbors method to add all of the neighbors of the current node to the open_list.
//  The NextNode() method to sort the open_list and return the next node.
//  When the search has reached the end_node the ConstructFinalPath method return the final path that was found.
//  Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = start_node;
  	open_list.push_back(start_node);
  	start_node->visited = true;
  	while(open_list.size() > 0){
      current_node = NextNode();
      if(current_node == end_node){
      	m_Model.path = ConstructFinalPath(current_node);
        break;
      }
      AddNeighbors(current_node);
    }
    return;
}