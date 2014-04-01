#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <ctime>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

string filename;
fstream input_file;

struct bnode{

	int key,degree;  //key is the value of a node, degree is the degree of a node
	bnode *fchild;   //the leftmost child of a node
	bnode *echild;	 //the rightmost child of a node
	bnode *next;	 //next node of a node
	bnode *pre;		 //previous node of a node
};

class binomial{

	private:
		//lnode *root;
	public:
		bnode* headnode;	//the first node of the first floor binomial heaps.
		bnode* lastnode;	//the last node of the first floor binomial heaps.
		bnode* b[50000];	//b[] is used to hold nodes to determine whether another node has the same degree
		int cdegree;		//a global parameter of keeping degree of a certain node
		string d[100000];	//used to newdisplay
		int x;				//index for d[]
		int level;			//keep the level of node in newdisplay
		int MaxKeysPerLevel;//the max number of keys per level
		bnode* nextone;		//a global parameter for analysis each nodes in the first floor of binomial heap



	binomial()	//contractor of binomial heap
	{
		nextone = NULL;
		headnode = NULL;
		lastnode = NULL;
		cdegree = 0;
		x = 0;
		level = 1;
		MaxKeysPerLevel = 10000;

		for(int j= 0; j < 50000; j++)
		{
			b[j] = NULL;
		}

		for(int j= 0; j < 100000; j++)
		{
			d[j] = "";
		}


	}




void insert(int newkey)  // to insert new nodes of binomial heap
{

	bnode* newnode = new bnode;
	newnode->key = newkey;
	newnode->fchild = NULL;
	newnode->echild = NULL;
	newnode->next = NULL;
	newnode->pre = NULL;
	newnode->degree = 0;

// if there is no node, set the new node as headnode and lastnode, otherwise add the new node after the lastnode
	if (headnode == NULL)
	{
		headnode = newnode;
		lastnode = newnode;
	}
	else
	{
		lastnode->next = newnode;
		newnode->pre = lastnode;
		lastnode = newnode;
	}
	
}

int deletemin()	//delete the min node of binomial heap
{

	bnode* minnode;
	bnode* next_x;

// if there is only one node in the first floor, set headnode and lastnode as NULL if the node has no child, or set fchild as headnode and echild as lastnode
	if((headnode == lastnode) && (headnode != NULL))
	{
		if(headnode->fchild == NULL)
		{
			headnode = NULL;
			lastnode = NULL;
		}
		else
		{	
			if(headnode->fchild == headnode->echild)
			{
				headnode->fchild->next = NULL;
				lastnode = headnode->fchild;
			}
			else
			{
				headnode->echild->next = NULL;
				lastnode = headnode->echild;
			}
			headnode->fchild->pre = NULL;
			headnode = headnode->fchild;
		}
	}
// if there is one more node in the first floor, go to find the minnode
	else if((headnode != NULL) && (headnode != lastnode))
	{
		minnode = headnode;
		next_x = headnode->next;

	    while(next_x != NULL)
	    {
	        if(minnode->key > next_x->key)
	        {
	        	minnode = next_x;
	        }
	        next_x = next_x->next;
	    }	
// after find the minimum node, do something to unconnect the minode with the binomial heap
// if minnode is not the headnode and there is 2 more nodes in the first floor
		    if((minnode == headnode) && (headnode->next != NULL))
		    {
		    	if(headnode->fchild == NULL)
		    	{
		    		headnode->next->pre = NULL;
		    		headnode = headnode->next;
		    	}
		    	else
		    	{
		    		headnode->echild->next = headnode->next;
		    		headnode->next->pre = headnode->echild;
		    		headnode->fchild->pre = NULL;
		    		headnode = headnode->fchild;
		    	}
		    	
		    }
// if there is only 1 node in the first floor.
		    else if((minnode == headnode) && (headnode->next == NULL))
		    {
		    	if(minnode->fchild == NULL)
		    	{
		    		headnode = NULL;
		    		lastnode = NULL;
		    	}
		    	else
		    	{
		    		headnode->fchild->pre = NULL;
		    		headnode->echild->next = NULL;
		    		headnode = headnode->fchild;
		    		lastnode = headnode->echild;
		    	}
		    }
// if the minnode is the lastnode
		    else if(minnode == lastnode)
		    {
		    	if(lastnode->fchild == NULL)
		    	{
		    		lastnode->pre->next = NULL;
		    		lastnode = lastnode->pre;
		    	}
		    	else
		    	{
		    		lastnode->pre->next = lastnode->fchild;
		    		lastnode->fchild->pre = lastnode->pre;
		    		lastnode->echild->next = NULL;
		    		lastnode = lastnode->echild;
		    	}
		    	
		    }
// if there is 3 or more nodes in the first floor, set something to unconnect the minnode with the binomial heap
		    else
		    {
		    	if(minnode->fchild == NULL)
		    	{

			    	minnode->pre->next = minnode->next;
			    	minnode->next->pre = minnode->pre;
		    	}
		    	else
		    	{
		    		minnode->pre->next = minnode->fchild;
		    		minnode->fchild->pre = minnode->pre;
		    		minnode->next->pre = minnode->echild;
		    		minnode->echild->next = minnode->next;
		    	}
		    }

		    minnode = NULL;
// after delete, if there is stll 2 or mode node, then calling the combinepair
		if((headnode != NULL) && (headnode != lastnode))
		{
			combinepair();
		}
		
	}
	else
	{
	}


}

//execute combine pair after delete the minimum node
void combinepair()	
{
// initiate the b[] array
	for(int j= 0; j < 50000; j++)
	{
		b[j] = NULL;
	}

    
    nextone = headnode->next;
    cdegree = headnode->degree;

    b[cdegree] = headnode;

// begin from the headnode to the lastnode, to evaluate whether any of nodes in the first floor has the same degree
    while(nextone != lastnode)
    {
        cdegree = nextone->degree;
        if(b[cdegree] != NULL)
        {
        	meld(b[cdegree],nextone);
        }
        else
        {
        	b[cdegree] = nextone;
        	nextone = nextone->next;
        }	
    }

    nextone = lastnode;

        cdegree = nextone->degree;
        if(b[cdegree] != NULL)
        {
        	//cout<<"combin4"<<endl;
        	meld(b[cdegree],nextone);
        }
        else
        {
        	b[cdegree] = nextone;
        }	

      
    
}

//meld two nodes
void meld(bnode* n1, bnode* n2)	
{
// if n1's key is smaller than n2's key, move n2 to n1's position
	if(n1->key < n2->key)
	{
// to reset the reationship of n1 and n2 between their neighboors
		if((n2->next != NULL) && (n2->pre != NULL))
		{
			n2->pre->next = n2->next;
			n2->next->pre = n2->pre;
		}
		else if((n2->next == NULL) && (n2->pre != NULL))
		{
			n2->pre->next = NULL;
			lastnode = n2->pre;
		}
		else if((n2->next != NULL) && (n2->pre == NULL))
		{
			n2->next->pre = NULL;
			headnode = n2->next;
		}
// set the nextone in order to aviod pointer wrong node after setting the next of n2
		if(n2->next != NULL)
		{
			nextone = n2->next;
		}
		else
		{
			nextone = NULL;
		}

// set the relationship between n1 and n2
		if(n1->fchild == NULL)
		{
			n1->fchild = n2;
			n1->echild = n2;
		}
		else if(n1->fchild != n1->echild)
		{	
			n1->echild->next = n2;
			n2->pre = n1->echild;
			n1->echild = n2;
		}
		else
		{	
			n1->fchild->next = n2;
			n2->pre = n1->fchild;
			n1->echild = n2;
		}
// after meld, the degree of n1 should be added 1
		n1->degree = n1->degree + 1;
		b[cdegree] = NULL;

		if(b[cdegree + 1] == NULL)
		{

			b[cdegree + 1] = n1;

		}
		else
		{
			cdegree = cdegree + 1;
			remeld(b[cdegree],n1);
		}

	}
// if n1's key is larger than or equal n2's key, move n2 to n1's position
	else
	{
// to reset the reationship of n1 and n2 between their neighboors
		if((n1->next != NULL) && (n1->pre != NULL))
		{

			n1->pre->next = n1->next;
			n1->next->pre = n1->pre;
		}
		else if((n1->next == NULL) && (n1->pre != NULL))
		{
			n1->pre->next = NULL;
			lastnode = n1->pre;
		}
		else if((n1->next != NULL) && (n1->pre == NULL))
		{
			n1->next->pre = NULL;
			headnode = n1->next;
		}
// set the nextone in order to aviod pointer wrong node after setting the next of n2
		if(n2->next != NULL)
		{
			nextone = n2->next;
		}
		else
		{
			nextone = NULL;
		}
// set the relationship between n1 and n2
		if(n2->fchild == NULL)
		{
			n2->fchild = n1;
			n2->echild = n1;
		}
		else if(n2->fchild != n2->echild)
		{
			n2->echild->next = n1;
			n1->pre = n2->echild;
			n2->echild = n1;
		}
		else
		{
			n2->fchild->next = n1;
			n1->pre = n2->fchild;
			n2->echild = n1;
		}
// after meld, the degree of n2 should be added 1
		n2->degree = n2->degree + 1;

		b[cdegree] = NULL;

		if(b[cdegree + 1] == NULL)
		{
			b[cdegree + 1] = n2;
		}
		else
		{
			cdegree = cdegree + 1;
			remeld(b[cdegree],n2);

		}

	}
}

//the only difference between meld is that remeld don't use nextnode to analyse the next node of the first floor of binomial heaps.
void remeld(bnode* n1, bnode* n2) 
{
// if n1's key is smaller than n2's key, move n2 to n1's position
	if(n1->key < n2->key)
	{
// to reset the reationship of n1 and n2 between their neighboors
		if((n2->next != NULL) && (n2->pre != NULL))
		{

			n2->pre->next = n2->next;
			n2->next->pre = n2->pre;
		}
		else if((n2->next == NULL) && (n2->pre != NULL))
		{
			n2->pre->next = NULL;
			lastnode = n2->pre;
		}
		else if((n2->next != NULL) && (n2->pre == NULL))
		{
			n2->next->pre = NULL;
			headnode = n2->next;
		}
// set the relationship between n1 and n2
		if(n1->fchild == NULL)
		{
			n1->fchild = n2;
			n1->echild = n2;
		}
		else if(n1->fchild != n1->echild)
		{	
			n1->echild->next = n2;
			n2->pre = n1->echild;
			n1->echild = n2;
		}
		else
		{	
			n1->fchild->next = n2;
			n2->pre = n1->fchild;
			n1->echild = n2;
		}
		// after meld, the degree of n1 should be added 1		
		n1->degree = n1->degree + 1;
		b[cdegree] = NULL;

		if(b[cdegree + 1] == NULL)
		{

			b[cdegree + 1] = n1;

		}
		else
		{
			cdegree = cdegree + 1;
			remeld(b[cdegree],n1);
		}

	}
// if n1's key is larger than or equal n2's key, move n2 to n1's position
	else
	{
// to reset the reationship of n1 and n2 between their neighboors
		if((n1->next != NULL) && (n1->pre != NULL))
		{

			n1->pre->next = n1->next;
			n1->next->pre = n1->pre;
		}
		else if((n1->next == NULL) && (n1->pre != NULL))
		{
			n1->pre->next = NULL;
			lastnode = n1->pre;
		}
		else if((n1->next != NULL) && (n1->pre == NULL))
		{
			n1->next->pre = NULL;
			headnode = n1->next;
		}


// set the relationship between n1 and n2
		if(n2->fchild == NULL)
		{
			n2->fchild = n1;
			n2->echild = n1;
		}
		else if(n2->fchild != n2->echild)
		{
			n2->echild->next = n1;
			n1->pre = n2->echild;
			n2->echild = n1;
		}
		else
		{
			n2->fchild->next = n1;
			n1->pre = n2->fchild;
			n2->echild = n1;
		}
		// after meld, the degree of n2 should be added 1
		n2->degree = n2->degree + 1;

		b[cdegree] = NULL;

		if(b[cdegree + 1] == NULL)
		{
			b[cdegree + 1] = n2;
		}
		else
		{
			cdegree = cdegree + 1;
			remeld(b[cdegree],n2);

		}

	}

}

// no use any more
void display()
{

}
// to display the results
void newdisplay()
{

	ofstream write("Binomial_heap.txt",ios::out);
	if(headnode != NULL)
	{
		bnode* pointer = new bnode;
		pointer = headnode;
		int counter = 0;
		string tempkey;
		string templev;
		string l1[MaxKeysPerLevel]; //set the string array to hold inforatmion for level 1
		string l2[MaxKeysPerLevel];
		string l3[MaxKeysPerLevel];
		string l4[MaxKeysPerLevel];
		string l5[MaxKeysPerLevel];
		string l6[MaxKeysPerLevel];
		string l7[MaxKeysPerLevel];
		string l8[MaxKeysPerLevel];
		string l9[MaxKeysPerLevel];
		string l10[MaxKeysPerLevel];
		string l11[MaxKeysPerLevel];
		string l12[MaxKeysPerLevel];
		string l13[MaxKeysPerLevel];
		string l14[MaxKeysPerLevel];
		string l15[MaxKeysPerLevel];
		int i1 = 0;	// set the index for level 1 string array
		int i2 = 0;
		int i3 = 0;
		int i4 = 0;
		int i5 = 0;
		int i6 = 0;
		int i7 = 0;
		int i8 = 0;
		int i9 = 0;
		int i10 = 0;
		int i11 = 0;
		int i12 = 0;
		int i13 = 0;
		int i14 = 0;
		int i15 = 0;

// pick up the level and key infoatmion from headnode to lastnode
		while(pointer != lastnode)
		{
			level = 1;
			d[x++] = convertInt(pointer->key);
			d[x++] = "(";
			
			d[x++] = convertInt(level);
			d[x++] = ") ";

			if(pointer->fchild != NULL)
			{
				level++;
				reload(pointer);
			}

			pointer = pointer->next;
		}

		level = 1;
		d[x++] = convertInt(lastnode->key);
		d[x++] = "(";
		
		d[x++] = convertInt(level);
		d[x++] = ") ";

		if(lastnode->fchild != NULL)
		{

			level++;
			reload(lastnode);
		}
//use d[] to write information into output file
		tempkey = d[counter];
		

		counter = counter + 1;

		while(d[counter] == "(")
		{	
			counter++;
			templev = d[counter];

			if(templev == "1")
			{
				l1[i1++] = tempkey;
			}
			else if(templev == "2")
			{
				l2[i2++] = tempkey;
			}
			else if(templev == "3")
			{
				l3[i3++] = tempkey;
			}
			else if(templev == "4")
			{
				l4[i4++] = tempkey;
			}
			else if(templev == "5")
			{
				l5[i5++] = tempkey;
			}
			else if(templev == "6")
			{
				l6[i6++] = tempkey;
			}
			else if(templev == "7")
			{
				l7[i7++] = tempkey;
			}
			else if(templev == "8")
			{
				l8[i8++] = tempkey;
			}
			else if(templev == "9")
			{
				l9[i9++] = tempkey;
			}
			else if(templev == "10")
			{
				l10[i10++] = tempkey;
			}
			else if(templev == "11")
			{
				l11[i11++] = tempkey;
			}
			else if(templev == "12")
			{
				l12[i12++] = tempkey;
			}
			else if(templev == "13")
			{
				l13[i13++] = tempkey;
			}
			else if(templev == "14")
			{
				l14[i14++] = tempkey;
			}
			else if(templev == "15")
			{
				l15[i15++] = tempkey;
			}

			counter = counter + 2;
			tempkey = d[counter];
			counter++;

		}

//begin to write key and level information into the output file
		write<<"(Bnomial heap)"<<endl;

		if(l1[0] != "")
		{
			write<<"Level 1:[";
			for(int j = 0; j < i1 - 1; j++)
			{
				write<<l1[j]<<",";
			}
			write<<l1[i1 - 1];
			write<<"]"<<endl;
			
		}
		if(l2[0] != "")
		{
			write<<"Level 2:[";
			for(int j = 0; j < i2 - 1; j++)
			{
				write<<l2[j]<<",";
			}
			write<<l2[i2 - 1];
			write<<"]"<<endl;
			
		}
		if(l3[0] != "")
		{
			write<<"Level 3:[";
			for(int j = 0; j < i3 - 1; j++)
			{
				write<<l3[j]<<",";
			}
			write<<l3[i3 - 1];
			write<<"]"<<endl;
			
		}
		if(l4[0] != "")
		{
			write<<"Level 4:[";
			for(int j = 0; j < i4 - 1; j++)
			{
				write<<l4[j]<<",";
			}
			write<<l4[i4 - 1];
			write<<"]"<<endl;
			
		}
		if(l5[0] != "")
		{
			write<<"Level 5:[";
			for(int j = 0; j < i5 - 1; j++)
			{
				write<<l5[j]<<",";
			}
			write<<l5[i5 - 1];
			write<<"]"<<endl;
			
		}
		if(l6[0] != "")
		{
			write<<"Level 6:[";
			for(int j = 0; j < i6 - 1; j++)
			{
				write<<l6[j]<<",";
			}
			write<<l6[i6 - 1];
			write<<"]"<<endl;
			
		}
		if(l7[0] != "")
		{
			write<<"Level 7:[";
			for(int j = 0; j < i7 - 1; j++)
			{
				write<<l7[j]<<",";
			}
			write<<l7[i7 - 1];
			write<<"]"<<endl;
			
		}
		if(l8[0] != "")
		{
			write<<"Level 8:[";
			for(int j = 0; j < i8 - 1; j++)
			{
				write<<l8[j]<<",";
			}
			write<<l8[i8 - 1];
			write<<"]"<<endl;
			
		}
		if(l9[0] != "")
		{
			write<<"Level 9:[";
			for(int j = 0; j < i9 - 1; j++)
			{
				write<<l9[j]<<",";
			}
			write<<l9[i9 - 1];
			write<<"]"<<endl;
			
		}
		if(l10[0] != "")
		{
			write<<"Level 10:[";
			for(int j = 0; j < i10 - 1; j++)
			{
				write<<l10[j]<<",";
			}
			write<<l10[i10 - 1];
			write<<"]"<<endl;
			
		}
		if(l11[0] != "")
		{
			write<<"Level 11:[";
			for(int j = 0; j < i11 - 1; j++)
			{
				write<<l11[j]<<",";
			}
			write<<l11[i11 - 1];
			write<<"]"<<endl;
			
		}
		if(l12[0] != "")
		{
			write<<"Level 12:[";
			for(int j = 0; j < i12 - 1; j++)
			{
				write<<l12[j]<<",";
			}
			write<<l12[i12 - 1];
			write<<"]"<<endl;
			
		}
		if(l13[0] != "")
		{
			write<<"Level 13:[";
			for(int j = 0; j < i13 - 1; j++)
			{
				write<<l13[j]<<",";
			}
			write<<l13[i13 - 1];
			write<<"]"<<endl;
			
		}
		if(l14[0] != "")
		{
			write<<"Level 14:[";
			for(int j = 0; j < i14 - 1; j++)
			{
				write<<l14[j]<<",";
			}
			write<<l14[i14 - 1];
			write<<"]"<<endl;
			
		}
		if(l15[0] != "")
		{
			write<<"Level 15:[";
			for(int j = 0; j < i15 - 1; j++)
			{
				write<<l15[j]<<",";
			}
			write<<l15[i15 - 1];
			write<<"]"<<endl;
			
		}
		
	}
	else
	{
		write<<"no element left..."<<endl;
		
	}

	write.close();


	
}

//to recursively pick up the child nodes of a certain nodes.
void reload(bnode* input)
{
	bnode* pointer = new bnode;
	bnode* child = new bnode;
	int templevel;
	

	pointer = input;
	child = pointer->fchild;

	while(child != pointer->echild)
	{	
		
		d[x++] = convertInt(child->key);
		d[x++] = "(";
		d[x++] = convertInt(level);
		d[x++] = ") ";
		if(child->fchild != NULL)
		{
			level++;
			reload(child);
		}
		
		child = child->next;
	}

	child = pointer->echild;

	
	d[x++] = convertInt(child->key);
	d[x++] = "(";
	d[x++] = convertInt(level);
	d[x++] = ") ";
		
	if(child->fchild != NULL)
	{
		level++;
		reload(child);
	}

	level--;
}

// convert int to string
string convertInt(int input)
{    
    int number = input;
    std::ostringstream ostr; 
    ostr << number; 
    std::string theNumberString = ostr.str(); 
	return  theNumberString;
}

// read commend from file
int read(){
  
	char input;
	int key;


	while(input_file.good())
	{
		input_file>>input;

		switch(input)
		{
			case 'I':
	  		input_file>>key;
			insert(key);
			break;
		
			case 'D':
			deletemin();
			break;
				
			case '*':
			break;
	
			default:
			break;
		}
	}
	input_file.close();
	return 1;
}

};




//----------------- leftist tree -----------------
struct lnode{

	int s,key,level;	// s the the s value of a node, key is the value of a node, level is the level of a node
	lnode *left;		// left child of a node
	lnode *right;		// right child of a node
};

class leftist{

	private:
		lnode *root;
	public:

	leftist()
	{
		root = NULL;
	}

//insert new node into the leftist tree
int insert(int newkey) {

	lnode* newnode = new lnode;
	newnode->key = newkey;
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->s = 1;
	root = meld(root,newnode);
	return 1;
}

//meld two leftist tree into one leftist tree
lnode* meld(lnode* firstnode, lnode* secondnode){

// samp the firstnode and the sencond if they are all avaible. otherwise return a NULL from one of them
	if((firstnode != NULL) && (secondnode == NULL))
	{
		return firstnode;
	}
	else if((firstnode == NULL) && (secondnode != NULL))
	{
		return secondnode;
	}
	else if((firstnode == NULL) && (secondnode == NULL))
	{
		return firstnode;
	}
	else if(firstnode->key > secondnode->key)
	{
		lnode* temp = secondnode;
		secondnode = firstnode;
		firstnode = temp;
	}
// if they n1 has a right child, then recursively meld the right child of firstnode with secondnode
	firstnode->right = meld(firstnode->right,secondnode);

// swamp the firstnode and second when the s value of firstnode is larger then the secondnode
	if((firstnode->left == NULL) && (firstnode->right !=NULL))
	{
		firstnode->left = firstnode->right;
		firstnode->right = NULL;
		firstnode->s = 1;
	}
	else if(firstnode->right->s > firstnode->left->s)
	{
		lnode* temp = firstnode->right;
		firstnode->right = firstnode->left;
		firstnode->left = temp;
		firstnode->s = firstnode->right->s + 1;
	}
	else
	{
		firstnode->s = firstnode->right->s + 1;
	}

	return firstnode;

}

//read operations from input file
int read(){
  
	char input;
	int key;


	while(input_file.good())
	{
		input_file>>input;

		switch(input)
		{
			case 'I':
	  		input_file>>key;
			insert(key);
			break;
		
			case 'D':
			deletemin();
			break;
				
			case '*':
			break;
	
			default:
			break;
		}
	}
	input_file.close();
	return 1;
}

//to display the result of leftist tree
int display(){

//create a file named Leftist_tree
	ofstream write("Leftist_tree.txt",ios::out);

	if(root == NULL){
		write<<"Leftist tree is empty"<<endl;
	}
	else
	{
		root->level = 0;
		lnode* n[100000];
		lnode* t;
		int lev = 0, nc = 0, tc = 0;

		for(int i = 0; i < 100000; i++){
			n[i] = NULL;
	}
	write<<"(Leftist tree)"<<endl<<"Level 1:[";

//display the key of node in each level
	while(root != NULL)
	{
		
		write << root->key;
		lev = root->level;

		if(root->left != NULL)
		{
			n[nc++] = root->left;
			root->left->level = lev + 1;
		}
		if(root->right != NULL)
		{
			n[nc++] = root->right;
			root->right->level = lev + 1;
		}

		t = n[tc];

		if(t != NULL){

			if(t->level == root->level){
				write << ",";
			}
			else
			{
				write << "]"<<endl<<"Level "<<t->level+1<<":[";
			}

			}
			root = n[tc++];
		}

		write<<"]";
	}

	write.close();
	

	return 1;
}

//delete the minimum nodes of leftist tree
void deletemin()
{
	if(root != NULL)
	{
	root = meld(root->left,root->right);
	}
}


};

// random mode
void randommode()
{	
	int m = 5000;
	int n = 5000;
	binomial b;
	leftist l;
	srand((int)time(0));
	clock_t StartB,TimeB;
	clock_t StartL,TimeL;

	int arrayA[n];
	int arrayB[m];
	int temp;
	int rank;

	int random;

	for(int i = 0; i < n; i++)
	{
		arrayA[i] = i;
	}

	for(int i = 0; i < n; i++)
	{	
		rank = rand()%n;
		temp = arrayA[i];
		arrayA[i] = arrayA[rank];
		arrayA[rank] = temp;
	}
	
	for(int i = 0; i < n; i++)
	{
		b.insert(arrayA[i]);
	}

	StartB = clock();
	for(int i = 0; i < m; i++)
	{
		random = rand()%n;
		switch(rand()%2)
		{
			case 0:
			b.insert(arrayA[random]);
			break;

			case 1:
			b.deletemin();
			break;
		}
	}
	TimeB = (double)(clock() - StartB) ;
	cout<<"Binomial Heap, Average Time Cost per Opeartion: "<<TimeB/(long (m))<<" micro seconds"<<endl;
	
	for(int k = 0; k < n; k++)
	{
		l.insert(arrayA[k]);
	}

	StartL = clock();
	for(int k = 0; k < m; k++)
	{
		random = rand()%n;
		switch(rand()%2)
		{
			case 0:
			l.insert(arrayA[random]);
			break;

			case 1:
			l.deletemin();
			break;
		}
	}
	TimeL = (double)(clock() - StartL) ;
	cout<<"Leftist Tree, Average Time Cost per Opeartion: "<<TimeL/(double(m))<<" micro seconds"<<endl;
}


int main(int argc,char **argv)
{
	binomial b;
	leftist l;

	if(argv[1]==NULL)
	{
  		cout<<"\nSpecify command : \n-r for Random mode \n-il <filename> for Min Leftist Tree\n-ib <filename> for Min Binomial Heap\n";
  		return 0;
  	}
	string input=argv[1];
 
	if(input.compare("-ib")==0) 
	{
		if(argv[2]==NULL)
   		{
   			cout<<"\nSpecify the input filename\n";
   			return 0;
   		}
		else
		{
			filename=argv[2];
			input_file.open(filename.c_str(),ios::in);
			if(!input_file)
			{
				cout<<"\nError opening file\n";
				return 1;
			}
			b.read();
			b.newdisplay();
			input_file.close();
  		}
	}

	else if(input.compare("-il")==0) 
	{
		if(argv[2]==NULL)
   		{
   			cout<<"\nSpecify the input filename\n";
   			return 0;
   		}
		else
		{
			filename=argv[2];
			input_file.open(filename.c_str(),ios::in);
			if(!input_file)
			{
				cout<<"\nError opening file\n";
				return 1;
			}
			l.read();
			l.display();
			input_file.close();
  		}
	}

	else if(input.compare("-r")==0) 
	{
		randommode();
	}
	else
		cout<<endl<<"Invalid command\n";
}

