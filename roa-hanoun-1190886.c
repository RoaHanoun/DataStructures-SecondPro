/* Department of Computer Science
           COMP2421
           Project #2
  ******** ROA HANOUN || 1190886 ********
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node
{
    int key; // Number of word
    char word [100];
    char meaning[100];
    struct node *left, *right; // pointer to left & right subtree
};

typedef struct node *BST;
struct node *root=NULL; // binary tree
FILE *f;

char *trim(char *str) // to delete the space from words
{
    char *end;

    while(isspace((unsigned char)*str))
        str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';

    return str;
}
// create new node in tree
BST newNode(int key, char *word, char *meaning)
{
    BST temp = (BST)malloc(sizeof(struct node));
    temp->key = key;
    strcpy(	temp->word,word);
    strcpy(temp->meaning,meaning);

    temp->left = temp->right = NULL;
    return temp;
}
// insert a new node to tree
BST insert(BST root, int key, char *word, char *meaning)
{
    /* If the tree is empty, return a new node */
    if (root == NULL)
        return newNode(key, word, meaning);
// insert to the left or right subtree according to its value of word
    if (strcmp(word,root->word) <0 )
        root->left = insert(root->left, key, word, meaning);
    else if (strcmp(word,root->word) >0)
        root->right = insert(root->right, key, word,meaning);

    return root;
}

//  Read dictionary.txt file and create the dictionary.
void ReadFile()
{
    FILE *fp;
    fp = fopen("Dictionary.txt", "r");
    f= fopen("Report.txt","w");

    char singleLine [200]; // to store one line from file dictionary
    char lines [200][200]; // store all the liens from file dictionary
    int numberoflines=0;

    while(!feof(fp))
    {
        fgets(singleLine,200, fp);
        if(strcmp(singleLine,"\n")!=0)
        {
            strcpy(lines[numberoflines],strtok(singleLine,"\n"));
            numberoflines++;
        }
    }

    char segments [200][200]; // to store each word and its meaning
    int numberofsegs=0;
///
    for(int i=0; i<numberoflines; i++)
    {
        char *token;
        token = strtok(lines[i],"\t");
        while(token !=NULL)
        {
            strcpy(segments[numberofsegs],token);
            numberofsegs++;
            token = strtok(NULL,"\t");
        }
    }
// insert all words with meaning in the tree ;
    for(int i=0; i<numberofsegs; i++)
    {
        if (strcmp(segments[i],"\n") !=0)
        {
            int number =atoi( strtok(segments[i],"."));
            char *name = strtok(NULL,":");
            char *meaning = strtok(NULL,"\0");
            root = insert(root, number,trim(name),meaning);
            fprintf(f,"%d. %s: %s\n",number,trim(name),meaning);


        }
    }
fprintf(f,"*********************************************************\n");
    fclose(fp);
}

// Print all words in the dictionary in an alphabetic order with their associated meanings.
void inorder(BST root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d. %s:%s \n", root->key,root->word,root->meaning);

        inorder(root->right);
    }
}
// get minimum word
BST minValueNode(BST node)
{
    BST current = node;

    // loop down to find the leftmost leaf
    while (current && current->left != NULL)
        current = current->left;

    return current;
}
//  Delete a word from the dictionary.
BST deleteNode(BST root, char *word)
{
    // if it empty tree
    if (root == NULL)
        return root;
/* Determining the location of the word by examining its value and
then deciding if it is in the left subtree or right subtree.*/
    if (strcmp(word,root->word) <0)
        root->left = deleteNode(root->left, word);

    else if (strcmp(word,root->word) >0)
        root->right = deleteNode(root->right, word);

    else
    {
        // one child or no child
        if (root->left == NULL)
        {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {

            struct node* temp = root->left;
            free(root);
            return temp;
        }
// with 2 children
        struct node* temp = minValueNode(root->right);// get minimum word from right subtree
        strcpy(root->word, temp->word);
        strcpy(root->meaning, temp->meaning);
        root->key=temp->key;
        root->right = deleteNode(root->right, temp->word);
    }
    return root;
}

// Search for a word and display its meaning.
BST find(BST root, char *word)
{
    while (root != NULL)
    {
        if (strcmp(word, root->word) > 0)

            return find(root->right, word);

        else if (strcmp(word, root->word) < 0)

            return find(root->left, word);

        else

            return root;

    }
    return NULL;
}
// Update the meaning of an existing word.
void Update(BST Head)
{
    char word[100];
    char meaning[100];
    printf("Please enter the word you want to update it: ");
    fflush(stdin);
    gets(word);
    fflush(stdin);
    BST fin = find(Head, word); // find the word to update its meaning
    if(fin == NULL)
    {
        printf("[This word does not exist!]\n");

        return;
    }
    printf("\t**WE FOUND IT**\n%s: %s\n", fin->word, fin->meaning); // print the word and its meaning
    printf("Please enter a new Meaning: ");
    fflush(stdin);
    gets(meaning);
    fflush(stdin);
    strcpy(fin->meaning, meaning);// copy the new meaning to the previous meaning
    printf("\nUpdated Successfully\n");

    return;
}
// Print all words that start with a specific character in an alphabetic order.
void print(BST root,char c)
{
    if(root != NULL)
    {
        print(root->left, c);// call the function to the left side
        if(root->word[0] == c)// cheek if the first character in the words same with specific character
        printf("%s: %s\n", root->word, root->meaning);// print it if we found it
        print(root->right, c);// call the function to the right side
    }


}
// Delete all words that start with a specific letter.
void delete_c(BST ro,char c)
{

    if(ro != NULL)
    {
        delete_c( ro -> left, c);
        if(ro->word[0] == c)
        {
            printf("%s\n", ro->word);
            root = deleteNode(root, ro->word);
            ro=root;
            delete_c(ro,c);
        }
        delete_c( ro -> right, c);
    }
}

int main()
{
    BST t;
    char str[128], meaning[256],c;
    int key,ch;
    while (1)
    {
        printf("******menu******\n");
        printf("\t1. Read file dictionary\n\t2. Insert word\n\t3. Delete word\n");
        printf("\t4. Find word\n\t5. Print-in order-\n\t6. Update word\n");
        printf("\t7. print all words with a specific character\n\t8. delete all words with a specific character\n\t9. Exit\nEnter your choice:");
        scanf("%d", &ch);
        getchar();
        switch (ch)
        {
        case 1:
            printf("\n");
            ReadFile();
            printf("\n");
            break;
        case 2:
            printf("\n");
            printf("Word to insert:");
            fgets(str, 100, stdin);
            str[strcspn(str, "\n")] = 0;
            printf("Meaning:");
            fgets(meaning, 256, stdin);
            meaning[strcspn(meaning, "\n")] = 0;
            printf("Key:");
            scanf("%d",&key);
            insert(root,key,str, meaning);
            printf("\n");
            break;

        case 3:
            printf("\n");
            printf("Please enter the word you want to delete it:");
            fgets(str, 100, stdin);
            deleteNode(root,str);
            printf("\n");
            break;

        case 4:
            printf("\n");
            printf("Enter the search word:");
            fgets(str, 100, stdin);
            str[strcspn(str, "\n")] = 0;
            t= find(root,str);
            if (t != NULL)
                printf("%d. %s:%s\n",t->key,t->word,t->meaning);
            else
                printf("NULL\n");
            printf("\n");
            break;

        case 5:
            printf("\n");
            inorder(root);
            printf("\n");
            break;

        case 6:
            printf("\n");
            Update(root);
            printf("\n");
            break;
        case 7:
            printf("\n");
            printf("Please enter the Character: ");
            fflush(stdin);
            scanf("%c", &c);
            print(root, c);
            printf("\n");
            break;
        case 8:
            printf("\n");
            printf("Please enter the Character: ");
            fflush(stdin);
            scanf("%c", &c);
            fflush(stdin);
            printf("\n***Deleted words***\n");
            delete_c(root, c);
            printf("\nThe deletion process completed successfully with character %c\n", c);
            printf("\n");

            break;
        case 9:
            exit(0);
        default:
            printf("\n");
            printf("You have entered wrong option!!\n");
            printf("\n");
            break;
        }
    }

    return 0;
}
