#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graph_utility.hpp>
#include <map>

using namespace std;
using namespace boost;

typedef adjacency_list<listS, vecS, undirectedS, property<vertex_name_t, string, property<vertex_priority_t, int> >, property<edge_name_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef map<string, Vertex> NameToVertex;

Vertex get_vertex(const string& name, Graph& G, NameToVertex& names)
{
	NameToVertex::iterator i = names.find(name);
	if (i == names.end())
		i = names.insert(make_pair(name, add_vertex(name, G))).first;
	return i->second;
}

int main(int argc, char* argv[])
{
	Graph G;
	NameToVertex names;
	
	if (argc == 2) {
		// load edges and nodes
		string in_source, in_target;
		ifstream edge_in;
		edge_in.open(argv[1]);
		if (!edge_in.fail()) {
			while (!edge_in.eof() && edge_in >> in_source >> in_target) {
				add_edge(get_vertex(in_source, G, names), get_vertex(in_target, G, names), G);
			}
			edge_in.close();
		} else {
			cerr << "Error: invalid edge file " << argv[1] << endl;
			return -1;
		}
		
		// calculate families/components
		property_map<Graph,vertex_name_t>::type thename = get(vertex_name, G);
		std::vector<int> g_component(num_vertices(G));
		int g_num = connected_components(G, &g_component[0]);
		cerr << "This graph of " << num_vertices(G) << " node(s) and " << num_edges(G) << " edge(s) has " << g_num << " component(s)" << endl;

		// print results
		graph_traits<Graph>::vertex_iterator ii, iend;
		for (tie(ii, iend) = vertices(G); ii != iend; ++ii) {
			cout << thename[*ii] << "\t" << g_component[*ii] << endl;
		}
		return 0;
	} else {
		cerr << "Usage: get_families <gene pairs file [gene1 tab gene2] >" << endl;
		return -1;
	}
}
/* g++ -Wall -g -O3 -march=core2 -pipe -msse4.2 -m64 -I/sw/include/boost-1_35/ -o find_gene_families find_gene_families.cpp */
