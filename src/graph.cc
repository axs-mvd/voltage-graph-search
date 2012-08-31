#include "graph.hh"

const vertex graph::null_vertex = -1;

graph::graph(uint32_t size, shared_ptr<group> g) {
    _size = size;
    _g = g;

    _adjacency_matrix = new alpha_set*[_size * _size];
    for (uint32_t i = 0; i < _size * _size; i++) {        
        _adjacency_matrix[i] = 0;
    }
}


graph::~graph() {
    if (_adjacency_matrix != 0) {
        for (uint32_t i = 0; i < _size * _size; i++) {        
            if (_adjacency_matrix[i] != 0) {
                delete _adjacency_matrix[i];
            }
        }
        delete [] _adjacency_matrix;
        _adjacency_matrix = 0;
    }
}


void graph::add_edge(const edge &e) {

    if (e.source() != graph::null_vertex && e.destination() != graph::null_vertex) {
        if (get(e.source(), e.destination()) == 0) 
            set(e.source(), e.destination(), new alpha_set);

        get(e.source(), e.destination())->push_back(e.alpha());

        if (get(e.destination(), e.source()) == 0) 
            set(e.destination(), e.source(), new alpha_set);

        get(e.destination(), e.source())->push_back(_g->inverse(e.alpha()));

    } else {
        vertex u = -1;
        if (e.source() != graph::null_vertex) u = e.source();
        else u = e.destination();

        if (get(u, u) == 0) 
            set(u, u, new alpha_set);
       
        get(u, u)->push_back(e.alpha()); 
    }
    
}




void graph::vertices(vector<vertex> &_vertices) const {
    for (vertex u = 0; u < (vertex)_size; u++) {
        _vertices.push_back(u);
    }
}

vector<vertex> graph::vertices() const {
    vector<vertex> _vertices;
    vertices(_vertices);
    return _vertices;
}


void graph::alphas(const vertex &u, const vertex &v, 
                vector<shared_ptr<group_element> > &a) const {

    if (get(u, v) != 0) {
        //_alphas.assign(get(u,v)->begin(), get(u,v)->end());
        a = *get(u,v);
    }
}

vector<shared_ptr<group_element> > graph::alphas(const vertex &u, const vertex &v) const {
    vector<shared_ptr<group_element> > _alphas;

    alphas(u, v, _alphas);

    return _alphas;
}

void graph::adjacents(const vertex &u, vector<vertex> &adj) const {
    for (vertex v = 0; v < (vertex)_size; v++) {
        if (get(u, v) != 0) {
            adj.push_back(v);
        }
    }
}

vector<vertex> graph::adjacents(const vertex &u) const {
    vector<vertex> adj;

    adjacents(u, adj);

    return adj;
}

uint32_t graph::size() const {
    return _size;
}

string graph::tostring() const {
    stringstream stream;
    for (vertex u = 0; u < (vertex)_size; u++) {
        for (vertex v = u; v < (vertex)_size; v++) {
            if (get(u,v) != 0) {
                stream << "("  << u << ", " << v << ") = " << endl; 

                alpha_set filtered = filter_inverses(*get(u,v));
                for(alpha_set::const_iterator alpha = filtered.begin(); 
                    alpha != filtered.end();  alpha++) {

                    stream << "\t" << alpha->get()->tostring() << endl;
                }
            }
        }
    }
    return stream.str();
}

