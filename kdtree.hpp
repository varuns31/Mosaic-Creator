/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    if(first[curDim]<second[curDim])return true;
    else if(first[curDim]==second[curDim])return first<second;
    else
    return false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    int distance1=0;
    int distance2=0;
    for(int i=0;i<Dim;i++)
    {
      distance1=pow(target[i]-currentBest[i],2)+distance1;
    }
    for(int i=0;i<Dim;i++)
    {
      distance2=pow(target[i]-potential[i],2)+distance2;
    }
    if(distance2<distance1)return true;
    else if(distance2==distance1)return potential<currentBest;
    else
    return false;
}
template <int Dim>
void KDTree<Dim>::swap(Point<Dim>& a, Point<Dim>&b)
{
  Point<Dim> temp=a;
  a=b;
  b=temp;
}

template <int Dim>
unsigned KDTree<Dim>::partition(vector<Point<Dim>>& list,unsigned left,unsigned right,unsigned pivotindex,int dim)
{
  Point<Dim> PivotValue=list[pivotindex];
  swap(list[pivotindex],list[right]);
  unsigned storeindex=left;
  for(unsigned i=left;i<right;i++)
  {
    if(smallerDimVal(list[i],PivotValue,dim))
    {
      swap(list[storeindex],list[i]);
      storeindex++;
    }
  }
  swap(list[right],list[storeindex]);
  return storeindex;
}

template <int Dim>
Point<Dim>& KDTree<Dim>::select(vector<Point<Dim>>& list,unsigned left,unsigned right,unsigned k,int dim)
{
  while(true!=false)
  {
  if(left==right)return list[left];
  unsigned pivotindex=(left+right)/2;
  pivotindex=partition(list,left,right,pivotindex,dim);
  if(k==pivotindex)return list[k];
  else if(k<pivotindex)right=pivotindex-1;
  else  left=pivotindex+1;
  }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildtree(vector<Point<Dim>>& newPoints, unsigned left, unsigned right, int currdim)
{
  if(left > right || left >= newPoints.size() || right >= newPoints.size())return NULL; 
  if(left == right) {
    KDTreeNode* temp = new KDTree<Dim>::KDTreeNode(newPoints[left]);
    temp->right = NULL;
    temp->left = NULL;
    return temp;
  }

  unsigned mid = (left + right) / 2;
  KDTreeNode* newroot = new KDTreeNode(select(newPoints, left, right, mid, currdim % Dim));
  size = size + 1;
  newroot->left = buildtree(newPoints, left, mid - 1, currdim + 1);
  newroot->right = buildtree(newPoints, mid + 1, right, currdim + 1);
  return newroot;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
  vector<Point<Dim>> newPoints2;
  size = 0;

  if(newPoints.empty()) {
    root = nullptr;
    return;
  }

  for(unsigned i = 0; i < newPoints.size(); i++) {
    newPoints2.push_back(newPoints[i]);
  }

  root = buildtree(newPoints2, 0, newPoints.size() - 1, 0);

}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copyhelper(KDTreeNode* OtherRoot)
{
  if(OtherRoot==NULL)return NULL;
  KDTreeNode* Root=new KDTreeNode();
  Root->point=OtherRoot->point;
  Root->left=(OtherRoot->left);
  Root->right=(OtherRoot->right);
  return Root;
}
template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  root=copyhelper(other.root);
  size=other.size;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  deletehelper(root);
  root=copyhelper(rhs.root);
  size=rhs.size;
  return *this;
}
template <int Dim>
void KDTree<Dim>::deletehelper(KDTreeNode * Root) 
{
  if(Root==NULL)return;
    if (Root->left != NULL) deletehelper(Root->left);
  if (Root->right != NULL) deletehelper(Root->right);
  delete Root;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  deletehelper(root);
}
template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
  return findNearestNeighborHelper(root, query, 0);
}
template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighborHelper(KDTreeNode* root, const Point<Dim>& query, int dim) const {

	//empty tree
	if (root->left == NULL && root->right == NULL) return root->point;

	Point<Dim> currentBest;
  int flag=0;

	//condition for left subtree
	if (smallerDimVal(query, root->point, dim)) 
  {
    flag=1;
		if (root->left != NULL) {
      currentBest = findNearestNeighborHelper(root->left, query, (dim + 1) % Dim);
    } else {
      currentBest = root->point;
    }
	}
	else if (!smallerDimVal(query, root->point, dim)) {
    flag=0;
		if (root->right != NULL) {
      currentBest = findNearestNeighborHelper(root->right, query, (dim + 1) % Dim);
    } else {
      currentBest = root->point;
    }
	}

	//swap
	if (shouldReplace(query, currentBest, root->point)) currentBest = root->point;
	
	//calculate dist
	double radius = 0;
	for (int i = 0; i < Dim; i++) radius = radius + (query[i] - currentBest[i]) * (query[i] - currentBest[i]);
	
	double splitDist = (root->point[dim] - query[dim]);
	splitDist = splitDist * splitDist;

	// Check if we need to traverse the other subtree or not
  Point<Dim> myNearest;

  if(radius == splitDist) {
    if(currentBest < root->point) {
      return currentBest;
    }
  }

	if (splitDist <= radius) {
    if(flag==1 && root->right != NULL)  myNearest = findNearestNeighborHelper(root->right, query, (dim + 1) % Dim);
    else if(flag==0 && root->left != NULL) myNearest = findNearestNeighborHelper(root->left, query, (dim + 1) % Dim);
    if (shouldReplace(query, currentBest, myNearest)) currentBest = myNearest;
	}

	return currentBest;
}