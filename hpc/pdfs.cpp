#include <iostream>
#include <omp.h>
#include <queue>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct TreeNode {
  int data;
  vector<TreeNode *> children;

  TreeNode(int val) { data = val; }
};


TreeNode* generateTree(int depth, int child_per_node, int& currentVal) {
    if(depth==0) return nullptr;

    TreeNode* root = new TreeNode(currentVal++);

    for(int i=0;i<child_per_node; i++){
        //this child can be null
        TreeNode* child = generateTree(depth-1, child_per_node, currentVal);
        if(child) {
            root->children.push_back(child);
        }
    }
    return root;
}

void dfs(TreeNode *root) {
  if (!root) return;

    cout<<root->data << " ";
    for(TreeNode* child: root->children) {
        dfs(child);
    }
}

void pbfs(TreeNode *root) {
  if (!root)
    return;

  vector<TreeNode *> q = {root};
    vector<int> op;

  while (!q.empty()) {
        int size = q.size();

        vector<TreeNode *> nextlvl;

        #pragma omp parallel 
        {

            vector<TreeNode *> localnext;
            vector<int> localop;

            #pragma omp for nowait
            for (int i = 0; i < size; i++) {

                TreeNode *curr = q[i];

                localop.push_back(curr->data);

                /* #pragma omp critical */
                /* { */
                /*     cout << "Thread " << omp_get_thread_num() << " processing node "<< curr->data << endl; */
                /* } */

                for (TreeNode *child : curr->children) {
                    localnext.push_back(child);
                }
            }

            #pragma omp critical
            {
                op.insert(op.end(), localop.begin(), localop.end());
                nextlvl.insert(nextlvl.end(), localnext.begin(), localnext.end());
            }
        }

        q = nextlvl;
    }

    cout<<"BFS traversal: ";
    for(int i: op){
        cout<<i<<" ";
    }
    cout<<endl;
}

int main() {

    int begin = 0;
  TreeNode *root = generateTree(3, 2, begin);
    auto start = high_resolution_clock::now();
    bfs(root);
    auto end = high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    auto pstart = high_resolution_clock::now();
    pbfs(root);
    auto pend = high_resolution_clock::now();
    auto pduration = chrono::duration_cast<chrono::milliseconds>(pend - pstart);

    cout<<"Total time required for normal bfs : "<< duration.count() <<" ms"<< endl;
    cout<<"Total time required for parallel bfs: "<< pduration.count() <<" ms"<< endl;
  return 0;
}
