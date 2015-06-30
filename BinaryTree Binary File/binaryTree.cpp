#include<iostream>
#include <fstream>
#include <vector>
#define RECORD_LENGTH  16
using namespace std ;
struct  binaryTree {
 
		int nodeValue;
		int refValue; 
		int left ; 
		int right ; 
		binaryTree(int nodeValue , 	int refValue , int left , int right){
			this->nodeValue = nodeValue; 
			this->refValue = refValue ; 
			this->left = left ; 
			this->right = right ;
			
		}	
};  

void CreateRecordsFile (char * filename , int numberOfRecords) {
			fstream  file;
			file.open("binaryTree.txt" , ios::out|ios::binary );
			binaryTree *record = new binaryTree (1,-1,0,0)  ; 
			file.write ((char*)&record , RECORD_LENGTH );
			for (int i = 2 ; i < numberOfRecords ; i++){		
		    record = new binaryTree (i,0,0,0)  ; 
			file.write ((char*)&record , RECORD_LENGTH );
		}
		 	record = new binaryTree (-1,0,0,0)  ; 
			file.write ((char*)&record , RECORD_LENGTH );
			file.close();
}

int InsertNewRecordAtIndex (char *filename , int recordID , int reference){
	
	 		binaryTree *record , *record2 ; 
			fstream file ;
			file.open(filename , ios::out|ios::in|ios::binary);
			
			file.read ((char*)&record , RECORD_LENGTH ); // read first column 
			int temp2 = record->nodeValue; // index of current node that inserted 
			int temp1 = record->refValue;
			if(temp2 == -1 ){
				cout<<"the  tree is full !!"<<endl;
				return 0 ; 
			}
			//read first node that will be put in it the next value to replace index 
			file.seekg ( (RECORD_LENGTH )* temp2, ios::beg);
			file.read ((char*)&record , RECORD_LENGTH ); 
			int temp3 = record->nodeValue ; // index of current node that inserted 
		 
			// add new node 
			file.seekg ( (RECORD_LENGTH )* temp2, ios::beg);
			record2 = new binaryTree ( recordID , reference , -1  , -1);
			file.write ((char*) &record2 , RECORD_LENGTH );	// write new node ;
			// update first coloum
			file.seekp (ios::beg);
			record2 = new binaryTree ( temp3 , 1 , 0  , 0);
			file.write ((char*) &record2 , RECORD_LENGTH );
			
			
			// update tree right and left after isert new node ; 
			
	 		file.seekg ( (RECORD_LENGTH ) , ios::beg);
	 		file.read ((char*)&record , RECORD_LENGTH ); 
	 		while (true){
	 				
	 				if(record->nodeValue < recordID){
					 
				 		if(record->right == 0 || record->right == -1 ){
				 	
				 			file.seekp ( (RECORD_LENGTH )*-1 , ios::cur);
				 			record2 = new binaryTree ( record->nodeValue , record->refValue , record->left , temp2 );
							file.write ((char*) &record2 , RECORD_LENGTH );	
						    break;
						    
				 		}else {
			
			 				file.seekg ( (RECORD_LENGTH )* record->right , ios::beg);
			 				file.read ((char*)&record , RECORD_LENGTH );
			 			}
			 		
			 		
			 		}else if ( record->nodeValue > recordID){
			 		 
			 			if(record->left == 0 || record->left == -1){
			 				
			 	 			file.seekp ( (RECORD_LENGTH )*-1 , ios::cur);
				 			record2 = new binaryTree ( record->nodeValue , record->refValue ,  temp2 , record->right );
							file.write ((char*) &record2 , RECORD_LENGTH );	
							break; 
							
						}else {
			 	
			 			file.seekg ( (RECORD_LENGTH )*record->left , ios::beg);
			 			
			 			file.read ((char*)&record , RECORD_LENGTH );
			 			}
						 	
	 				}else   break ; 		// here if  the tree has only one node 	
		}
}

 void DeleteRecordFromIndex (char *filename, int recordID){
	
	binaryTree * record , *record3 , *record4  ;
	int index = 1 , counter = 1 , lastIndex =1  , c = 0;
	fstream file ;
    file.open(filename , ios::out|ios::in|ios::binary);
	file.seekg ( (RECORD_LENGTH ) , ios::beg);
	file.read ((char*)&record , RECORD_LENGTH );
	while (true){
		
		if( record-> nodeValue == recordID ){
 			 
			if( record->right == -1 && record->left ==  -1 ){ //check if it is leaf	
		//	cout<<index<<" is "<<endl;
			  
				file.seekp ( (RECORD_LENGTH )* index , ios::beg ) ; 
				binaryTree *record2 = new binaryTree(-1 , 0 , 0 , 0) ;
		    	file.write ((char*) &record2 , RECORD_LENGTH ) ;
		    	
			    // update nodeValue of empty records ;
			    
				file.seekg (0,ios::beg);
				file.read ((char*)&record3 , RECORD_LENGTH );
				while (true ){
					
					if(record3->nodeValue == -1){
							
						file.seekp ( (RECORD_LENGTH )* -1, ios::cur); // to update target parent 
						binaryTree *record2 = new binaryTree(index  , 0 , 0 , 0);
				    	file.write ((char*) &record2 , RECORD_LENGTH );
				    	break;	 
					}else {
						file.seekg( (RECORD_LENGTH )*record3->nodeValue , ios::beg);
						file.read ((char*)&record3 , RECORD_LENGTH );
						}
					}
				// update node that was pointer to deleted node !
				//cout<<lastIndex <<" FF "<<index <<endl;
				file.seekg ( (RECORD_LENGTH ) * lastIndex, ios::beg);  
				file.read ((char*)& record3 , RECORD_LENGTH );
				if(record3->right == index ){
					file.seekp ( (RECORD_LENGTH )* -1, ios::cur);  
					binaryTree *record2 = new binaryTree(record3->nodeValue , record3->refValue, record3->left , -1);
			    	file.write ((char*) &record2 , RECORD_LENGTH );
					break;
				}else if(record3->left == index){
					file.seekp ( (RECORD_LENGTH )* -1 , ios::cur);  
					binaryTree *record2 = new binaryTree(record3->nodeValue , record3->refValue , -1  , record3->right );
			    	file.write ((char*) &record2 , RECORD_LENGTH );
			    	break;
								    	
				}else break;
				break ;
				// not leave ,, has right or left 	
			}else if(record->right == -1 || record->left == -1){
				
				binaryTree *newRecord = record  , *tempRecord , *tempRecord2 ;
			    // update parent deleted node .. 
			    
		//	 cout<<"hey"<<newRecord->nodeValue<<" "<<index<<"  "<<newRecord->right<<" "<<endl;
			 /// delete it 
			    file.seekg( (RECORD_LENGTH ) * index  , ios::beg ) ;
				file.read ((char*)&record3 , RECORD_LENGTH ); // read   9
				int leftNode = record3->left; 
				int rightNode = record3->right; 
			//	cout<<"yes "<< leftNode <<" S"<<rightNode <<endl;
				file.seekp( (RECORD_LENGTH ) * -1  , ios::cur ) ;
				binaryTree *record5 = new binaryTree(-1 ,0 , 0 ,0);
			    file.write ((char*) &record5, RECORD_LENGTH );
				int node = 0 ; 
				if(rightNode == -1 )node = leftNode; 
				else node = rightNode ; 
				 binaryTree* record6;
				 
				 // read record that was pointers  by deleted node 
				 file.seekg( (RECORD_LENGTH ) * node , ios::beg);
				 file.read ((char*)&record6 , RECORD_LENGTH );
				 // delete record that was pointer 
				 file.seekp( (RECORD_LENGTH ) * -1 , ios::cur);
				 binaryTree *record7 = new binaryTree(-1 ,0 , 0 ,0);
				 file.write ((char*) &record7, RECORD_LENGTH );
				 
				// write  node that was pointer by deleted node  insted of deleted node place 
				 file.seekp( (RECORD_LENGTH ) * index , ios::beg);
				 file.write ((char*) &record6 , RECORD_LENGTH );
				// updat parent 
				binaryTree* record4 ; 
				file.seekg( (RECORD_LENGTH ) * lastIndex , ios::beg) ;
				file.read ((char*)&record4 , RECORD_LENGTH );
				//cout<<record4->nodeValue<<"  "<<record4->left <<"  "<<record->right <<"  " <<endl;
				
				
				
				if(record4->left == index )record4->left =  index ; 
				else record4->right =  index ; 
				
				file.seekp( (RECORD_LENGTH ) * -1  , ios::cur ) ;
				file.write ((char*) &record4 , RECORD_LENGTH );	
			 
				// update first empty nodes 
				
				file.seekg (0,ios::beg);
				file.read ((char*)&record3 , RECORD_LENGTH );
				while (true ){
					
					if(record3->nodeValue == -1){
							
						file.seekp ( (RECORD_LENGTH )* -1, ios::cur); // to update target parent 
						binaryTree *record2 = new binaryTree(node  , 0 , 0 , 0);
				    	file.write ((char*) &record2 , RECORD_LENGTH );
				    	break;	 
					}else {
						file.seekg( (RECORD_LENGTH )*record3->nodeValue , ios::beg);
						file.read ((char*)&record3 , RECORD_LENGTH );
						}
					}
		 
				break ; 
							
			}else if (record->right != -1 && record->left != -1 ){
					//cout<<"wwwwwwwww"<<endl;
				int x = 0 , z;
				binaryTree *newRecord = record  , *tempRecord = record , *tempRecord2 ;
				
				int index3 = 1 , y = record->right , temp1, temp2 ;//i = 2; 
			//	index = newRecord->right ; 
				// get the most left from right node and update its parent ; 
				file.seekg ( (RECORD_LENGTH )* newRecord->right , ios::beg); // to right 
				//cout<<"dfd: "<<newRecord->right<<"  "<<index3<<endl;
				while(true ){
					
					file.read ((char*)&record3 , RECORD_LENGTH ); 
					if(record3->left == -1  ){  //  found most left leaf from right node ;
						
						file.seekp( (RECORD_LENGTH )* index3 , ios::beg);  
					//	cout<<"hhh "<<tempRecord->nodeValue<<"  "<< tempRecord->refValue<<endl;		 
						binaryTree *record2 = new binaryTree(tempRecord->nodeValue , tempRecord->refValue, -1 , tempRecord->right);
			    		file.write ((char*) &record2 , RECORD_LENGTH );
						tempRecord2 = record3 ;
						break; 	
					}
					//   index3 : index of node that pointered to node that pointer to  next parent node
					index3 = y ; 
				    y = record3->left;
				    z = record3->right ;
				    x++; 	 
				    
				 	tempRecord = record3 ;  // keep  record  of parent of leaf node that will be parent  .. to update its left  ;  
				 //	i = tempRecord->left;
					file.seekg ( (RECORD_LENGTH )* record3->left , ios::beg); // to leaft ;
				}
				//  'record3' or newRecord is the node that will be next parent insted of deleted node 
			   //   'record'  is the node that i will deleted ; 
			   				 
		     // delete place of nextparent node to inserted in  parent node that deleted  
		     	 // i = tempRecord->left;
		     	//cout<<"S "<<i<<endl;
				file.seekp( (RECORD_LENGTH )* y , ios::beg); 
				binaryTree *record2 = new binaryTree( -1 , 0 , 0 , 0 );
			    file.write ((char*) &record2 , RECORD_LENGTH );
				   
			    // update the first column after delete (next empty node)  ; 
			    
			  file.seekg (0,ios::beg);
			  file.read ((char*)&record4 , RECORD_LENGTH );
				while (true ){	
			//	cout<<"ww "<<tempRecord->right<<endl;
					
					if(record4->nodeValue == -1){
						
						file.seekp ( (RECORD_LENGTH )* -1, ios::cur); // to update target parent 
						binaryTree *record2 = new binaryTree(y , 0 , 0 , 0);
				    	file.write ((char*) &record2 , RECORD_LENGTH );
				    	break;						 
					}else {
						file.seekg( (RECORD_LENGTH )*record4->nodeValue , ios::beg);
						file.read ((char*)&record4 , RECORD_LENGTH );
						}				
					}
			    // update place of parent node that you   deleted  ;
			   // cout<<x<<"XX " <<newRecord->right<<endl;
			    file.seekp( (RECORD_LENGTH )* index , ios::beg); 
			    if(x == 0){
			    record2 = new binaryTree( tempRecord2->nodeValue, tempRecord2->refValue , newRecord->left, tempRecord2->right );
			
				}else  record2 = new binaryTree( tempRecord2->nodeValue, tempRecord2->refValue , newRecord->left, newRecord->right );
			    file.write ((char*) &record2 , RECORD_LENGTH );
				break;
			}
		}else if(record->nodeValue < recordID){
			counter++;
			lastIndex = index ;
			index = record->right ; // keep the index of target ;
		 	 if(record->right == -1){
		 	 	cout<<endl;
		  	cout<<recordID<<" NOt Found !! "<<endl;
			break ;	
		  	}
			file.seekg ( (RECORD_LENGTH )* record->right , ios::beg);
			file.read ((char*)&record , RECORD_LENGTH );
 		}else if ( record->nodeValue > recordID){
 			counter++;
 			lastIndex = index ;
 			index =record->left; // keep the index of target 
 			if(record->left == -1){
 				cout<<endl;
		  	cout<<recordID <<"  NOt Found !! "<<endl;
			break ;	
		  	}
 		//	c += record->left ;
 			file.seekg ( (RECORD_LENGTH )*record->left , ios::beg);
 			file.read ((char*)&record , RECORD_LENGTH );
		}else{
				 
			cout<<"NOt Found !! "<<endl;
			break ;		
		}
		
	
	//	break;
}
file.close();
}
 


void  inorder(char * fileName , binaryTree * p){
 		fstream file ;		
		file.open(fileName , ios::in|ios::binary);
		//if(p->left == -1 && p->right == -1 )return ;
		if(p->left != -1){
			
        	file.seekg ((RECORD_LENGTH ) *p->left , ios::beg);
        	file.read((char*)&p , RECORD_LENGTH);
        	inorder(fileName , p );	
        }
        
        cout <<p->nodeValue<<" ";
        
        if( p->right != -1  ){
        	
        	//	file.open(fileName , ios::in|ios::binary);
        		file.seekg ( (RECORD_LENGTH ) * p->right  ,  ios::beg ) ;
        		file.read((char*)&p , RECORD_LENGTH);
        		inorder(fileName, p );
        } 
       // else return  ;
    	file.close();
} 
 
void DisplayBinarySearchTreeInOrder (char *filename ){
	binaryTree  *record   ;
	fstream file ;
	file.open(filename , ios::in|ios::binary);
	file.read((char*)& record , RECORD_LENGTH);
	file.read((char*)& record , RECORD_LENGTH);
//	file.close();
	inorder(filename,record);
//	DisplayBinarySearchTreeInOrder (filename, 1);
		}
 
void DisplayIndexFileContent (char *filename) {
	binaryTree  *record   ;
	fstream file ;
	file.open(filename , ios::in|ios::binary);
	file.read((char*)&record , RECORD_LENGTH);
	while (true){
			if(file.eof()) break ;
	cout << record->nodeValue << "," << record->refValue << "," << record->left <<","<<record->right <<endl;
	file.read((char*)&record , RECORD_LENGTH);

//
	}
	file.close();
}
	
int main () {
	CreateRecordsFile ("binaryTree.txt", 8 );
	DisplayIndexFileContent ("binaryTree.txt");
	
	
	InsertNewRecordAtIndex("binaryTree.txt" , 5 , 12 );
	InsertNewRecordAtIndex("binaryTree.txt" , 8 , 24 );
	InsertNewRecordAtIndex("binaryTree.txt" , 9 , 36 );
	InsertNewRecordAtIndex("binaryTree.txt" , 3 , 48 );
	InsertNewRecordAtIndex("binaryTree.txt" , 1 , 60 );
	InsertNewRecordAtIndex("binaryTree.txt" , 6 , 72 );
	InsertNewRecordAtIndex("binaryTree.txt" ,7, 84 );
	cout<< endl ;
   DisplayIndexFileContent ("binaryTree.txt");
	cout<< endl ;
 	DeleteRecordFromIndex ("binaryTree.txt" , 9);
 		DeleteRecordFromIndex ("binaryTree.txt" , 3);
 			DeleteRecordFromIndex ("binaryTree.txt" , 5);
	
	 DisplayIndexFileContent ("binaryTree.txt");
//	 DisplayBinarySearchTreeInOrder ("binaryTree.txt" );
	
 /*  	DeleteRecordFromIndex ("binaryTree.txt" , 8);
	DeleteRecordFromIndex ("binaryTree.txt" , 4);
 	 
 	InsertNewRecordAtIndex("binaryTree.txt" , 15 , 96);
 	InsertNewRecordAtIndex("binaryTree.txt" , 13 , 108);
	
	DeleteRecordFromIndex ("binaryTree.txt" , 12);
	DeleteRecordFromIndex ("binaryTree.txt" , 5);
	 
 	DeleteRecordFromIndex ("binaryTree.txt" ,3);
    DeleteRecordFromIndex ("binaryTree.txt" ,13 );
	cout<<endl;
	DisplayIndexFileContent ("binaryTree.txt");*/
	
	
	return 0 ; 
}
