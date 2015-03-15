#include <stdio.h>
#include <math.h>

//example design parameters
#define TOTAL_MEM_BLOCKS 8*1024*1024
#define MAX_TREE_DEPTH 23
#define TREE_RAM_LENGTH 2396745
#define ALVEC_RAM_LENGTH TOTAL_MEM_BLOCKS/32

//direction
#define UP 1
#define DOWN 0

#define GD 30 //group depth, for example

//my types
typedef int word;

typedef struct BlockCoordi {
	int verti;
	int horiz;
} coordi;

typedef struct BlockScope {
	int request_size;
	coordi coo;
	int direction;
	int pnode_sel; 
	int pnode_sel_phy;
	int search_status; 
	word tree_block;
	int row_base;
	int saddr;
	int alvec;
	int group_addr;
	int virtual_gaddr;
} scope;

typedef struct AllocationDrone {
	int request_size;
	coordi coo;
	int direction;
	int row_base;
	int alvec;
	int pnode_sel;
	int saddr;
	int original_reqsize;
	int flag_markup;
	int node_or;
	int node_and;
} drone;

typedef struct MarkupDrone{
	int node_or;
	int node_and;
	coordi coo;
	int done;
	int row_base;
} mupdrone;

typedef struct FreeInfo{
	coordi coo;
	int pnode_sel;
	int row_base;	
	int alvec;
	int group_addr;
	int virtual_gaddr;
}freeprobe;

typedef struct HolderType{
	int group[32];
}holder;

typedef struct get_coo_type{
	coordi coo;
	int row_base;
	int topsize;
}getcoo_type;

word bram[TREE_RAM_LENGTH];
word vec_bram[ALVEC_RAM_LENGTH];
int flag_use_alvector;
int flag_first;
int flag_alloc;
int free_vcheck;
int held_start_verti;
int flag_failed;
int flag_blocking;

int read_count;
int write_count;

holder held_mtree[GD];
int held_pnode_sel[GD];
int held_address[GD];

int overlord[MAX_TREE_DEPTH + 1];

int original_verti;

int test_index;

//buddy allocations
int alloc(int request_size);
void de_alloc(int saddr,int reqsize);
scope locate_block(scope input);
drone mark_allocation_down(drone input);
mupdrone mark_allocation_up(mupdrone input);
freeprobe get_free_info(int saddr,int reqsize);

//hw functions
int bram_read(int address);
void bram_write(int address, int content);
void bram_init(int length);
void vector_init(int length);
int vector_read(int address);
void vector_write(int address, int content);
void check_alvector(void);
void update_group(int *mtree,int alvec);
void malloc_update(int size, int group_addr);
scope scope_gen(int size);
getcoo_type get_coo(int addr, int size);
int get_index(int size);
scope check_blocking(scope input);
scope check_blocking_prep(scope input);

int sizegen(int input);

//sw functions
void tree_map(int *tree_block_i,word tree_block);
word tree_mapback(int *mtree);
void copy_mtree(int *input, holder *output, int index);
void copy_mtree_direct(int *input, int *output);
void ptree(int address);
void pvec(int address);
void pgroup(int *input);