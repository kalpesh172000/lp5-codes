#include <chrono>
#include <iostream>
#include <omp.h>
#include <queue>
#include <vector>

using namespace std;
using namespace std::chrono;

struct TreeNode {
    int data;
    vector<TreeNode *> children;

    TreeNode(int val) { data = val; }
};

TreeNode *generateTree(int depth, int child_per_node, int &currentVal) {
    if (depth == 0)
        return nullptr;

    TreeNode *root = new TreeNode(currentVal++);

    for (int i = 0; i < child_per_node; i++) {
        // this child can be null
        TreeNode *child = generateTree(depth - 1, child_per_node, currentVal);
        if (child) {
            root->children.push_back(child);
        }
    }
    return root;
}

void dfs(TreeNode *root) {
    if (!root)
        return;

    cout << root->data << " ";
    for (TreeNode *child : root->children) {
        dfs(child);
    }
}


void pdfs(TreeNode* root, int depth = 0)
{
    if(!root) return;

    #pragma omp critical
    cout<< root-> data << " by thread"<<omp_get_thread_num()<<endl;

    for(TreeNode* child: root->children){
        if(depth<4){
            #pragma omp task
            pdfs(child, depth + 1);
        }
        else {
            pdfs(child, depth + 1);
        }
    }
    #pragma omp taskwait
}

int main() {

    int begin = 0;
    TreeNode *root = generateTree(10, 5, begin);
    auto start = high_resolution_clock::now();
    dfs(root);
    auto end = high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout<<"\n";

    //interesting observation
    omp_set_num_threads(12);

    cout<<"parelled dfs : ";
    auto pstart = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        {
            pdfs(root);
        }
    }
    auto pend = high_resolution_clock::now();
    cout<<"\n";
    cout<<"max threads used : "<<omp_get_max_threads()<<endl;
    auto pduration = chrono::duration_cast<chrono::milliseconds>(pend - pstart);

    cout << "Total time required for normal dfs : " << duration.count() << " ms"
         << endl;
    cout << "Total time required for parallel dfs: " << pduration.count()
         << " ms" << endl;
    return 0;
}
