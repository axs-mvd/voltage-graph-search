UTRACER_HOME=../../utracer
UTRACER_INCLUDE=$(UTRACER_HOME)/include
UTRACER_LIB=$(UTRACER_HOME)/lib

CXXFLAGS = -g -pg -pipe -W -Wall -pthread -pedantic -fPIC   #profilling
#CXXFLAGS = -g -pipe -W -Wall -pthread -pedantic -fPIC  #debug
#CXXFLAGS = -O3 -pipe -W -Wall -pthread -pedantic -fPIC  #release
CXXLINK = $(CXXFLAGS) -lrt 
#LIBFLAGS= -L$(UTRACER_LIB) -lutracer

################################################################################
## Objects
################################################################################

OBJECTS = graph.o lift.o bfs_queue.o random_search_thread.o generator.o 

BINARIES_DIR=../bin

################################################################################
## Executable objects
################################################################################

LIFT_TEST_OBJECT = lift_test.o
ZMXZN_TEST_OBJECT = zmxzn_test.o
BIG_BITSET_TEST_OBJECT=test_bitset.o
BFS_QUEUE_TEST_OBJECT=bfs_queue_test.o
RANDOM_SEARCH_OBJECT=random_search.o
SRS_OBJECT=sequential_random_search.o
TEST_GENERATE_TUPLE_OBJECT=test_generate_tuple.o
TEST_GENERATOR_OBJECT=test_generator.o
MPGL_OBJECT=mpgl.o

################################################################################
## Executable binaries
################################################################################

LIFT_TEST_EXECUTABLE = $(BINARIES_DIR)/lift_test
ZMXZN_TEST_EXECUTABLE = $(BINARIES_DIR)/zmxzn_test
BIG_BITSET_TEST_EXECUTABLE = $(BINARIES_DIR)/bitset_test
BFS_QUEUE_TEST_EXECUTABLE = $(BINARIES_DIR)/bfs_queue_test
RANDOM_SEARCH_EXECUTABLE = $(BINARIES_DIR)/random_search
SRS_EXECUTABLE = $(BINARIES_DIR)/srs
TEST_GENERATE_TUPLE_EXECUTABLE=$(BINARIES_DIR)/generate_tuple
TEST_GENERATOR_EXECUTABLE=$(BINARIES_DIR)/generator
MPGL_EXECUTABLE=$(BINARIES_DIR)/mpgl

################################################################################
## Executable binaries
################################################################################

ALL_EXEC = $(LIFT_TEST_EXECUTABLE) $(ZMXZN_TEST_EXECUTABLE) \
		   $(BIG_BITSET_TEST_EXECUTABLE) $(BFS_QUEUE_TEST_EXECUTABLE) \
		   $(RANDOM_SEARCH_EXECUTABLE) $(TEST_GENERATE_TUPLE_EXECUTABLE) \
		   $(TEST_GENERATOR_EXECUTABLE) $(MPGL_EXECUTABLE) \
		   $(SRS_EXECUTABLE)

################################################################################

all: make_bin_dir $(ALL_EXEC) 

################################################################################
$(LIFT_TEST_EXECUTABLE): $(OBJECTS) $(LIFT_TEST_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(ZMXZN_TEST_EXECUTABLE): $(OBJECTS) $(ZMXZN_TEST_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(BFS_QUEUE_TEST_EXECUTABLE): $(OBJECTS) $(BFS_QUEUE_TEST_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(BIG_BITSET_TEST_EXECUTABLE) : $(BIG_BITSET_TEST_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(RANDOM_SEARCH_EXECUTABLE) : $(OBJECTS) $(RANDOM_SEARCH_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(TEST_GENERATE_TUPLE_EXECUTABLE) : $(OBJECTS) $(TEST_GENERATE_TUPLE_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(TEST_GENERATOR_EXECUTABLE) : $(OBJECTS) $(TEST_GENERATOR_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(MPGL_EXECUTABLE) : $(OBJECTS) $(MPGL_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

$(SRS_EXECUTABLE) : $(OBJECTS) $(SRS_OBJECT)
	$(CXX) $(CXXLINK) $^ $(LIBFLAGS) -o $@

################################################################################
clean: clear

make_bin_dir: 
	mkdir -p $(BINARIES_DIR)

clear:
	@echo "cleaning..."
	@cd $(BINARIES_DIR) && rm -f $(ALL_EXEC) 

rebuild: clear all
