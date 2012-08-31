#include "lift.hh"

#include <queue>
#include <set>
#include "big_bitset.hh"
#include "bfs_queue.hh"

vector<lift_vertex> lift::adjacents(const lift_vertex &u_g) const {

    vector<lift_vertex> adjacents_lift;
    adjacents(u_g, adjacents_lift);

    return adjacents_lift;
}
void lift::adjacents(const lift_vertex &u_g, vector<lift_vertex> &adjacents_lift) const {
    vertex u_gamma = u_g.v();         
    shared_ptr<group_element> g = u_g.g();

    vector<vertex> adjacents;

     _gamma->adjacents(u_gamma, adjacents);

    vertex v_gamma;
    vector<shared_ptr<group_element> > alphas;

    for (vector<vertex>::iterator adjacent = adjacents.begin(); adjacent != adjacents.end(); adjacent++) {
        v_gamma = *adjacent;
        _gamma->alphas(u_gamma, v_gamma, alphas);

        for (vector<shared_ptr<group_element> >::iterator alpha = alphas.begin(); alpha != alphas.end(); alpha++) {
            shared_ptr<group_element> h = _group->operation(g, *alpha);
            lift_vertex v_h(v_gamma, h);
            adjacents_lift.push_back(v_h);
        }
        alphas.clear();
    }
}

int lift::delta() const {

    vector<vertex> vertices = _gamma->vertices();
    uint32_t max_delta = 0;
    uint32_t current_delta = 0;
    vector<vertex> adjacents;
    vector<shared_ptr<group_element> > alphas;

    for (vector<vertex>::const_iterator u = vertices.begin(); u != vertices.end(); u++) {
        adjacents = _gamma->adjacents(*u);
        current_delta = 0;
        for (vector<vertex>::const_iterator v = adjacents.begin(); v != adjacents.end(); v++) {
            alphas = _gamma->alphas(*u, *v);

            for (vector<shared_ptr<group_element> >::iterator alpha = alphas.begin(); 
                alpha != alphas.end(); alpha++) {
                current_delta++;
            }
        }

        if (current_delta > max_delta) {
            max_delta = current_delta;
        }
    }

    return max_delta;
}



int lift::diameter() const {

    timestamp_probe("LIFT::DIAMETER::BEGIN"); 

    vector<vertex>vertices = _gamma->vertices();
    int _max_distance = 0;
    for (vector<vertex>::const_iterator v = vertices.begin(); 
            v != vertices.end(); v++) {
        lift_vertex u_g(*v, _group->random_element()); 
        int current_max_distance = max_distance(u_g);

        if (current_max_distance > _max_distance) {
            _max_distance = current_max_distance;            
        }
    }
    timestamp_probe("LIFT::DIAMETER::END"); 

    return _max_distance;
}

bool lift::check_diameter(const int &expected_diameter) const {

    timestamp_probe("LIFT::CHECK_DIAMETER::BEGIN"); 
    bool check = true;
    vector<vertex>vertices = _gamma->vertices();

    for (vector<vertex>::const_iterator v = vertices.begin(); 
            v != vertices.end() && check; v++) {
        lift_vertex u_g(*v, _group->random_element()); 
        if (!check_distance(u_g, expected_diameter)) 
            check = false;
    }

    timestamp_probe("LIFT::CHECK_DIAMETER::END"); 

    return check;
}

int lift::bfs(const lift_vertex &u, const int &accepted_max_distance) const {

    timestamp_probe("LIFT::MAXDISTANCE::QUEUE::CREATE::BEGIN"); 
    bfs_queue  *q = bfs_queue_factory::create_queue(size());
    timestamp_probe("LIFT::MAXDISTANCE::QUEUE::CREATE::END"); 

    timestamp_probe("LIFT::MAXDISTANCE::VISITED::CREATE::BEGIN"); 
    big_bitset visited(size());
    timestamp_probe("LIFT::MAXDISTANCE::VISITED::CREATE::END"); 

    uint64_t lift_vertex_index = index_of(u);
    uint16_t distance = 0;

    bfs_queue_element e(lift_vertex_index, distance); 
    q->push(e);
    visited.set(lift_vertex_index);

    uint16_t _max_distance = 0;
    uint64_t visited_count = 1;
    uint64_t iteration = 0;
    lift_vertex v = u;
    vector<lift_vertex> adj;

    timestamp_probe("LIFT::MAXDISTANCE::COMPUTE::BEGIN"); 
    while (!q->empty()) {

        if ((accepted_max_distance != -1) && 
            (_max_distance > accepted_max_distance)) {
            break;
        }

        iteration++;
        e = q->pop();
        v = get_vertex(e.lift_vertex_index());
        distance = e.distance() + 1;
        adjacents(v, adj);

        for (vector<lift_vertex>::const_iterator it = adj.begin();
                it != adj.end(); it++) {

            lift_vertex_index = index_of(*it); 
            if (!visited.get(lift_vertex_index)) {

                visited.set(lift_vertex_index);
                visited_count++;

                if (_max_distance < distance) {
                    _max_distance = distance;
                }

                if (accepted_max_distance != -1 && 
                    _max_distance > accepted_max_distance) {
                    break;
                }

                e.set_lift_vertex_index(lift_vertex_index);
                e.set_distance(distance);

                q->push(e);
            }
        }
        adj.clear();
    }
    timestamp_probe("LIFT::MAXDISTANCE::COMPUTE::END"); 

    delete q;

    bool is_connected = (visited_count == size());

    if (is_connected) return _max_distance;
    else return numeric_limits<int>::max();
}

int lift::max_distance(const lift_vertex &u) const {
   return bfs(u, -1); 
}

bool lift::check_distance(const lift_vertex &u, const int &distance) const {
    return bfs(u, distance) <= distance;
}

