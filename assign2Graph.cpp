//To compile: g++ -std=c++11 -O3 -w assign2Graph.cpp -lpthread -o assign2Graph
//To run: ./assign2Graph filename threads
//./assign2Graph networkDatasets/toyGraph1.txt 2
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

typedef unsigned vertex;
typedef vector<set<vertex> > Graph;


/* Global variables */
Graph G;
pthread_barrier_t barr;
pthread_mutex_t mutex; /* for global_sum */
double global_sum = 0; /* protected by mutex */
unsigned thread_count;


/* Serial functions */
Graph& read_graph(char filename[]);
void print_graph(Graph *g);
double clustering_coefficient(vertex u, Graph *g);


Graph& read_graph(char filename[])
{
     fstream f(filename, std::ios_base::in);
     vertex u,v;
     vector<pair<vertex,vertex> > all_edges;
     unsigned max_node = 0;
     while (f >> u >> v)
     {
          all_edges.push_back(make_pair(u,v));
          if (u > max_node)
          {
               max_node = u;
          }
          if (v > max_node)
          {
               max_node = v;
          }
     }
     Graph* g = new Graph(max_node + 1);
     for (unsigned i = 0; i < all_edges.size(); i++)
     {
          u = all_edges[i].first;
          v = all_edges[i].second;
          g->at(u).insert(v);
          g->at(v).insert(u);
     }
     f.close();
     return *g;
}


void print_graph(Graph *g)
{
     for (unsigned i=0; i<g->size(); i++)
     {
          cout << i << ": {";
          for (const vertex &s : g->at(i)) {
               cout << s << ", ";
          }
          cout << "}" << endl;
     }
}


/*
  Global in vars: G, thread_count, global_sum, mutex, barr
 */
void* do_work(void *rank)
{
     long my_rank = (long) rank;
     double local_sum = 0.0;
     unsigned rows_per_thread = G.size() / thread_count;
     unsigned begin = rows_per_thread * my_rank;
     unsigned end = (my_rank + 1) * rows_per_thread;

     for (unsigned i = begin; i < end; i++)
     {
          double C_i = clustering_coefficient(i, &G);
          local_sum += C_i;
     }

     pthread_mutex_lock(&mutex);
     global_sum += local_sum;
     pthread_mutex_unlock(&mutex);

     int rc = pthread_barrier_wait(&barr);

     if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
     {
          cout << "Could not wait on barrier" << endl;
          exit(-1);
     }

     return NULL;
}


double clustering_coefficient(vertex u, Graph *g)
{
     set<vertex> G_u = g->at(u);
     if (G_u.size() < 2)
     {
          return 0;
     }

     unsigned G_u_edges = 0;
     for (const vertex &v : G_u)
     {
          vector<vertex> isect;
          set<vertex> G_v = g->at(v);
          set_intersection(G_u.begin(),
                           G_u.end(),
                           G_v.begin(),
                           G_v.end(),
                           inserter(isect, isect.begin()));
          G_u_edges += isect.size();
     }
     unsigned n = G_u.size();
     unsigned total_possible_edges = (n * (n - 1));
     double C_u = (double)G_u_edges / total_possible_edges;
     return C_u;
}


int main(int argc, char** argv)
{
     if(argc<3){
          cout<<"To run: ./assign2Graph filename threads"<<endl;
          cout<<"./assign2Graph networkDatasets/toyGraph1.txt 2"<<endl;
          return 0;
     }

     G = read_graph(argv[1]);
     thread_count = atoi(argv[2]);
     if(thread_count < 2) {
          for (unsigned i = 0; i < G.size(); i++)
          {
               double C_i = clustering_coefficient(i, &G);
               global_sum += C_i;
          }
          double C_G = global_sum / G.size();
          cout << C_G << endl;
          return 0;
     }

     if (pthread_barrier_init(&barr, NULL, thread_count))
     {
          cout << "Could not create a barrier." << endl;
          return -1;
     }
     if (pthread_mutex_init(&mutex, NULL))
     {
          cout << "Could not create a mutex." << endl;
          return -1;
     }

     pthread_t thread_handles[thread_count];
     for (long i = 0; i < thread_count; i++)
     {
          pthread_create(&thread_handles[i], NULL, do_work, (void*)i);
     }
     for (long i = 0; i < thread_count; i++)
     {
          pthread_join(thread_handles[i], NULL);
     }

     double C_G = global_sum / G.size();
     cout << C_G << endl;

     pthread_mutex_destroy(&mutex);
     pthread_barrier_destroy(&barr);

     return 0;
}
