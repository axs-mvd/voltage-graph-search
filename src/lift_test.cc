#include <iostream>
#include <vector>
#include <exception>

#include <sys/time.h>

#include "lift.hh"
#include "graph.hh"
#include "zn.hh"
#include "zmxzn.hh"


/** returns time in milliseconds */
long tick() {
    struct timeval start;
    gettimeofday(&start, NULL);
    return (long)(start.tv_sec*1000 + (int)start.tv_usec/1000);
}


shared_ptr<graph> create_bell(shared_ptr<group> g) {
    shared_ptr<graph> gamma(new graph(2, g));

    vertex u = 0;
    vertex v = 1;

    gamma->add_edge(edge::make_edge(u, v, new zn_member(0)));
    
    gamma->add_edge(edge::make_edge(u, u, new zn_member(1)));
    gamma->add_edge(edge::make_edge(v, v, new zn_member(2)));

    return gamma;
}

void test_bouquet_eleven_ten() {
    uint32_t m = 368349;
    uint32_t n = 198;
    uint32_t r = 113;

    uint32_t expected_degree   = 11;
    uint32_t expected_diameter = 9;
    uint64_t expected_order    = 72933102;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;
    //semiedge
    gamma->add_edge(edge::make_edge(u, graph::null_vertex, new zmxzn_member(28382,99)));

    //(27703,196)(62272,94)(308948,123)(32154,32)(62308,149)]

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(27703,196)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(62272,94)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(308948,123)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(32154,32)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(62308,149)));


    lift gamma_alpha(gamma, g);

    uint64_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[bouquet eleven ten]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}

void test_bouquet_nine_nine() {
    uint32_t m = 168379;
    uint32_t n = 72;
    uint32_t r = 2242;

    uint32_t expected_degree   = 9;
    uint32_t expected_diameter = 9;
    uint64_t expected_order    = 12123288;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;
    //semiedge
    gamma->add_edge(edge::make_edge(u, graph::null_vertex, new zmxzn_member(70579,36)));

    //(133512,24)(107217,34)(59314,52)(66721,49)]

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(133512,24)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(107217,34)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(59314,52)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(66721,49)));


    lift gamma_alpha(gamma, g);

    uint64_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[bouquet nine nine]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}

void test_bouquet_eight_ten() {
    uint32_t m = 341057;
    uint32_t n = 73;
    uint32_t r = 2103;

    uint32_t expected_degree   = 8;
    uint32_t expected_diameter = 10;
    uint64_t expected_order    = 24897161;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    //(310448,53)(177761,43)(68031,31)(35532,28)]

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(310448,53)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(177761,43)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(68031,31)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(35532,28)));


    lift gamma_alpha(gamma, g);

    uint64_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[bouquet eight ten]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}


void test_k4_eleven_ten() {
    uint32_t m = 341125;
    uint32_t n = 440;
    uint32_t r = 157;

    uint32_t expected_degree = 11;
    uint32_t expected_diameter = 10;
    uint32_t expected_order = 600380000;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(4, g));
    
    vertex u = 0;
    vertex v = 1;
    vertex w = 2;
    vertex x = 3;

    //[(31691,411)][(0,0)(99550,185)(60165,251)
    //              (0,0)(231334,349)(313099,87)
    //              (0,0)(112517,278)(160342,268)]
    //[(157467,433)][(323365,254)(16622,346)(326002,374)
    //               (269993,262)(83914,219) (25515,415)]
    //[(36341,105)][(131753,366)(210675,185)(324856,343)]
    //[(93609,232)]

    //u loop
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(31691,411)));

    //u->v
    gamma->add_edge(edge::make_edge(u, v, new zmxzn_member(0,0)));
    gamma->add_edge(edge::make_edge(u, v, new zmxzn_member(99550,185)));
    gamma->add_edge(edge::make_edge(u, v, new zmxzn_member(60165,251)));

    //              (0,0)(231334,349)(313099,87)
    //u->w
    gamma->add_edge(edge::make_edge(u, w, new zmxzn_member(0,0)));
    gamma->add_edge(edge::make_edge(u, w, new zmxzn_member(231334,349)));
    gamma->add_edge(edge::make_edge(u, w, new zmxzn_member(313099,87)));
    
    //              (0,0)(112517,278)(160342,268)]
    //u->x
    gamma->add_edge(edge::make_edge(u, x, new zmxzn_member(0,0)));
    gamma->add_edge(edge::make_edge(u, x, new zmxzn_member(112517,278)));
    gamma->add_edge(edge::make_edge(u, x, new zmxzn_member(160342,268)));

    //starting in v
    //[(157467,433)][(323365,254)(16622,346)(326002,374)
    //               (269993,262)(83914,219) (25515,415)]
    //v loop
    gamma->add_edge(edge::make_edge(v, v, new zmxzn_member(157467,433)));

    //v->w
    gamma->add_edge(edge::make_edge(v, w, new zmxzn_member(323365,254)));
    gamma->add_edge(edge::make_edge(v, w, new zmxzn_member(16622,346)));
    gamma->add_edge(edge::make_edge(v, w, new zmxzn_member(326002,374)));

    //v->x
    gamma->add_edge(edge::make_edge(v, x, new zmxzn_member(269993,262)));
    gamma->add_edge(edge::make_edge(v, x, new zmxzn_member(83914,219))); 
    gamma->add_edge(edge::make_edge(v, x, new zmxzn_member(25515,415)));

    //starting in w
    //[(36341,105)]
    //[(131753,366)(210675,185)(324856,343)]

    //w loop
    gamma->add_edge(edge::make_edge(w, w, new zmxzn_member(36341,105)));

    //w->x
    gamma->add_edge(edge::make_edge(w, x, new zmxzn_member(131753,366)));
    gamma->add_edge(edge::make_edge(w, x, new zmxzn_member(210675,185))); 
    gamma->add_edge(edge::make_edge(w, x, new zmxzn_member(324856,343)));

    //x loop
    //[(93609,232)]
    gamma->add_edge(edge::make_edge(x, x, new zmxzn_member(93609,232)));


    lift gamma_alpha(gamma, g);

    uint32_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();


    cout << "_______________________________" << endl;
    cout << "[k4 eleven ten]" << endl;


    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}

void test_bouquet_seven_six() {
    uint32_t m = 333;
    uint32_t n = 36;
    uint32_t r = 2;

    uint32_t expected_degree = 7;
    uint32_t expected_diameter = 6;
    uint32_t expected_order = 11988;

    shared_ptr<group> g(new zmxzn(m, n, r));

    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    //semiedge
    gamma->add_edge(edge::make_edge(u, graph::null_vertex, new zmxzn_member(108,18)));

    //loops
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(61,14)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(195,34)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(14,23)));

    cout << "gamma:" << endl;
    cout << gamma->tostring() << endl;


    lift gamma_alpha(gamma, g);

    uint32_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();


    cout << "_______________________________" << endl;
    cout << "[bouquet seven six]" << endl;


    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}

void test_k3_4_10() {
    uint32_t m = 281;  
    uint32_t n = 21;  
    uint32_t r = 59;  

    uint32_t expected_diameter = 10;
    uint32_t expected_degree   = 4;
    uint32_t expected_order    = 17703;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(3, g));
    lift gamma_alpha(gamma, g);

    vertex u,v,w;
    u = 0;
    v = 1;
    w = 2;

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(160, 4)));   
    gamma->add_edge(edge::make_edge(u, v, new zmxzn_member(0, 0)));   
    gamma->add_edge(edge::make_edge(u, w, new zmxzn_member(0, 0)));   
  
    gamma->add_edge(edge::make_edge(v, v, new zmxzn_member(119, 1)));   
    gamma->add_edge(edge::make_edge(v, w, new zmxzn_member(179, 10)));   
    gamma->add_edge(edge::make_edge(w, w, new zmxzn_member(67, 13)));   
 
    cout << "gamma:" << endl;
    cout << gamma->tostring() << endl;

   
 
    uint32_t real_order = gamma_alpha.size();
    uint32_t real_degree = gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[K_3 4 10 test]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << "\t [ok]" << endl;
    else cout << "\t [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << "\t [ok]" << endl;
    else cout << "\t [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << "\t [ok]" << endl;
    else cout << "\t [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}   


void test_bell_z5_petersen() {
    shared_ptr<zn> group(new zn(5));

    shared_ptr<graph> gamma = create_bell(group);

    lift gamma_alpha(gamma, group);

    shared_ptr<group_element> g(new zn_member(0));
    lift_vertex u_g(1, g);
    vector<lift_vertex> adj;
    adj = gamma_alpha.adjacents(u_g);
    cout << "<" << u_g.v() << "," << u_g.g()->tostring() << ">" << endl;
    for (vector<lift_vertex>::iterator it = adj.begin();
         it != adj.end(); it++) {
        cout << "\t<" << it->v() << "," << it->g()->tostring() << "> " << endl;
    }

    uint32_t expected_order = 10;
    uint32_t expected_degree = 3;
    uint32_t expected_diameter = 2;

    uint32_t real_order = gamma_alpha.size();
    uint32_t real_degree = gamma_alpha.delta();
    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[bell z5 petersen test]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << "\t [ok]" << endl;
    else cout << "\t [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << "\t [ok]" << endl;
    else cout << "\t [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << "\t [ok]" << endl;
    else cout << "\t [fail]" << endl;

    cout << "diameter compute took: " << pos - prev << "ms" << endl;
}

void test_bouquet_14_10() {
    uint32_t m = 62316337;
    uint32_t n = 113;
    uint32_t r = 8864;

    uint32_t expected_degree   = 14;
    uint32_t expected_diameter = 10;
    uint64_t expected_order    = 7041746081;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    //[(4821549,46)(58679374,110)(29318275,6)(25838660,1) (41798940,9)(25321982,81) (47965471,21)]

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(4821549,46)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(58679374,110)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(29318275,6)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(25838660,1)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(41798940,9)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(25321982,81)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(47965471,21)));


    lift gamma_alpha(gamma, g);

    uint64_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[bouquet fourteen ten]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}


void test_bouquet_ten_ten() {
    uint32_t m = 531849;
    uint32_t n = 378;
    uint32_t r = 3620;

    uint32_t expected_degree   = 10;
    uint32_t expected_diameter = 10;
    uint64_t expected_order    = 201038922;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    ///(45789,159)(82505,117)(47344,316)(287530,344)(443446,190)

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(45789,159)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(82505,117)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(47344,316)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(287530,344)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(443446,190)));


    lift gamma_alpha(gamma, g);

    uint64_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[bouquet fourteen ten]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}

void test_bouquet_six_ten() {
    uint32_t m = 22793;
    uint32_t n = 55;
    uint32_t r = 72;

    uint32_t expected_degree   = 6;
    uint32_t expected_diameter = 10;
    uint64_t expected_order   = 1253615;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    //[(21654,34)(11504,49)(22520,44)]

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(21654,34)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(11504,49)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(22520,44)));


    lift gamma_alpha(gamma, g);

    uint64_t real_order = gamma_alpha.size();
    uint32_t real_degree= gamma_alpha.delta();

    long prev = tick();
    uint32_t real_diameter = gamma_alpha.diameter();
    long pos = tick();

    cout << "_______________________________" << endl;
    cout << "[bouquet six ten]" << endl;

    cout << "gamma_alpha.size() = " << real_order;
    if (real_order == expected_order) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.delta() = " << real_degree;
    if (real_degree == expected_degree) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;

    cout << "gamma_alpha.diameter() = " << real_diameter;
    if (real_diameter == expected_diameter) cout << " [ok]" << endl;
    else cout << " [fail]" << endl;
    cout << "diameter compute took: " << pos - prev << "ms" << endl;

    cout << "check_diameter ";
    if (gamma_alpha.check_diameter(expected_diameter)) 
        cout << "[ok]" << endl;
    else cout << "[fail]" << endl;

}

void generate_adjacencies_big() {
    uint32_t m = 358051;
    uint32_t n = 77;
    uint32_t r = 1626;

//    uint32_t expected_degree   = 8;
//    uint32_t expected_diameter = 10;
//    uint64_t expected_order    = 27569927;
//

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    //[(21654,34)(11504,49)(22520,44)]

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(802256,  63)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(1129089, 61)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(707865,  10)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(819021,  47)));


    lift gamma_alpha(gamma, g);

    for (uint64_t i = 0; i < gamma_alpha.size(); i++) {
        lift_vertex u_g = gamma_alpha.get_vertex(i); 

        cout << " ";
        vector<lift_vertex> adj = gamma_alpha.adjacents(u_g);
        for (vector<lift_vertex>::iterator a = adj.begin(); a != adj.end(); a++) {
            cout << gamma_alpha.index_of(*a) << " ";
        }
        cout << endl;
    } 
}

void generate_adjacencies_small3() {
    uint32_t m = 333;
    uint32_t n = 36;
    uint32_t r = 2;

//    uint32_t expected_degree = 7;
//    uint32_t expected_diameter = 6;
//    uint32_t expected_order = 11988;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    //semiedge
    gamma->add_edge(edge::make_edge(u, graph::null_vertex, new zmxzn_member(108,18)));

    //loops
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(61,14)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(195,34)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(14,23)));


    lift gamma_alpha(gamma, g);

    for (uint64_t i = 0; i < gamma_alpha.size(); i++) {
        lift_vertex u_g = gamma_alpha.get_vertex(i); 

        cout << " ";
        vector<lift_vertex> adj = gamma_alpha.adjacents(u_g);
        for (vector<lift_vertex>::iterator a = adj.begin(); a != adj.end(); a++) {
            cout << gamma_alpha.index_of(*a) << " ";
        }
        cout << endl;
    } 

}

void generate_adjacencies_small2() {
    uint32_t m = 333;
    uint32_t n = 36;
    uint32_t r = 2;

//    uint32_t expected_degree = 7;
//    uint32_t expected_diameter = 6;
//    uint32_t expected_order = 11988;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    //semiedge
    gamma->add_edge(edge::make_edge(u, graph::null_vertex, new zmxzn_member(108,18)));

    //loops
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(61,14)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(195,34)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(14,23)));


    lift gamma_alpha(gamma, g);

    for (uint64_t i = 0; i < gamma_alpha.size(); i++) {
        lift_vertex u_g = gamma_alpha.get_vertex(i); 

        cout << " ";
        vector<lift_vertex> adj = gamma_alpha.adjacents(u_g);
        for (vector<lift_vertex>::iterator a = adj.begin(); a != adj.end(); a++) {
            cout << gamma_alpha.index_of(*a) << " ";
        }
        cout << endl;
    } 

}

void generate_adjacencies_small() {

    uint32_t m = 281;  
    uint32_t n = 21;  
    uint32_t r = 59;  

    shared_ptr<group> g(new zmxzn(m, n, r));

    shared_ptr<graph> gamma(new graph(3, g));
    lift gamma_alpha(gamma, g);

    vertex u,v,w;
    u = 0;
    v = 1;
    w = 2;

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(160, 4)));   
    gamma->add_edge(edge::make_edge(u, v, new zmxzn_member(0, 0)));   
    gamma->add_edge(edge::make_edge(u, w, new zmxzn_member(0, 0)));   
  
    gamma->add_edge(edge::make_edge(v, v, new zmxzn_member(119, 1)));   
    gamma->add_edge(edge::make_edge(v, w, new zmxzn_member(179, 10)));   
    gamma->add_edge(edge::make_edge(w, w, new zmxzn_member(67, 13)));   
    
    for (uint64_t i = 0; i < gamma_alpha.size(); i++) {
        lift_vertex u_g = gamma_alpha.get_vertex(i); 

        cout << " ";
        vector<lift_vertex> adj = gamma_alpha.adjacents(u_g);
        for (vector<lift_vertex>::iterator a = adj.begin(); a != adj.end(); a++) {
            cout << gamma_alpha.index_of(*a) << " ";
        }
        cout << endl;
    } 
}

void generate_adjacencies_small4() {
    uint32_t m = 923;
    uint32_t n = 21;
    uint32_t r = 48;

/*    uint32_t m = 937;
    uint32_t n = 24;
    uint32_t r = 14;
*/


//    uint32_t expected_degree = 6;
//    uint32_t expected_diameter = 7;
////    uint32_t expected_order = 22488;
//    uint32_t expected_order = 19383;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

/*    //loops
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(241,9)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(733,0)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(360,15)));
*/

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(554, 7)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(204,12)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(850,4)));


    lift gamma_alpha(gamma, g);

    for (uint64_t i = 0; i < gamma_alpha.size(); i++) {
        lift_vertex u_g = gamma_alpha.get_vertex(i); 

        cout << " ";
        vector<lift_vertex> adj = gamma_alpha.adjacents(u_g);
        for (vector<lift_vertex>::iterator a = adj.begin(); a != adj.end(); a++) {
            cout << gamma_alpha.index_of(*a) << " ";
        }
        cout << endl;
    } 
}

void generate_adjacencies_candidate() {
    uint32_t m = 159;
    uint32_t n = 52;
    uint32_t r = 2;

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(147, 26)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(41, 14)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(112, 47)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(82, 37)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(113, 10)));

    lift gamma_alpha(gamma, g);

    for (uint64_t i = 0; i < gamma_alpha.size(); i++) {
        lift_vertex u_g = gamma_alpha.get_vertex(i); 

        vector<lift_vertex> adj = gamma_alpha.adjacents(u_g);
        for (vector<lift_vertex>::iterator a = adj.begin(); a != adj.end(); a++) {
            cout << i << " adj " << gamma_alpha.index_of(*a) << endl;
        }
    } 
}

void generate_adjacencies_candidate2() {
    uint32_t m = 6763;
    uint32_t n = 49;
    uint32_t r = 41; 

    shared_ptr<group> g(new zmxzn(m, n, r));
    shared_ptr<graph> gamma(new graph(1, g));
    
    vertex u = 0;

    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(1254, 25)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(541, 18)));
    gamma->add_edge(edge::make_edge(u, u, new zmxzn_member(4642, 47)));

    lift gamma_alpha(gamma, g);

    for (uint64_t i = 0; i < gamma_alpha.size(); i++) {
        lift_vertex u_g = gamma_alpha.get_vertex(i); 

        vector<lift_vertex> adj = gamma_alpha.adjacents(u_g);
        for (vector<lift_vertex>::iterator a = adj.begin(); a != adj.end(); a++) {
            cout << " " << gamma_alpha.index_of(*a);
        }
        cout << endl;
    } 
}


int main() {

//    test_bell_z5_petersen();  //10
//    test_bouquet_seven_six(); //11 988;
//    test_k3_4_10();           //17 703;
//    test_bouquet_six_ten();   //1 253 615;
//    test_bouquet_nine_nine(); //12 123 288;
////    test_bouquet_eight_ten(); //24 897 161
//    test_bouquet_eleven_ten(); //72 933 102
//    test_bouquet_ten_ten();   //201 038 922;
//    test_k4_eleven_ten();     //600 380 000;
//    test_bouquet_14_10();     //7 041 746 081;

//    generate_adjacencies_small();
//    generate_adjacencies_small4();
//    generate_adjacencies_candidate();
    generate_adjacencies_candidate2();
    return 0;
}
