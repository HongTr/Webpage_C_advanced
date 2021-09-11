#include "Graphlib.h"
typedef struct pageRank{
    int key;
    int output;
    float rank;
    float rank_temp;
}pageRank;
int main(){
    FILE* web, *connect;
    web = fopen("webpages.txt", "r");
    connect = fopen("pageConnections.txt", "r");
    if (web == NULL || connect == NULL){
        printf("Error!\n");
        exit(1);
    }
    Graph ID_tree = createGraph();
    Graph URL_tree = createGraph();



    int choice, flag = 0;
    do{
        printf("=====================================\n");
        printf("1. Chuc nang 1\n");
        printf("2. Chuc nang 2\n");
        printf("3. Chuc nang 3\n");
        printf("4. Chuc nang 4\n");
        printf("5. Chuc nang 5\n");
        printf("6. Thoat\n");
        printf("=====================================\n");
        printf("Lua chon cua ban: ");
        scanf("%d", &choice);
        if (choice <= 0 || choice >= 7) printf("Nhap khong hop le!\n");
        else if (choice >=1 && choice <=6){
            switch (choice){
                case 1:{
                    flag = 1;

                    //Add dinh
                    int so_luong_dinh;
                    fscanf(web, "%d\n", &so_luong_dinh);
                    int ID;
                    char URL[255];
                    for (int i = 1; i <= so_luong_dinh; i++){
                        fscanf(web, "%s %d\n", &URL, &ID);
                        addVertex(URL_tree, i, strdup(URL));
                        addVertex_int(ID_tree, ID, i);
                    }

                    //Add canh
                    int so_luong_canh;
                    fscanf(connect, "%d\n", &so_luong_canh);
                    
                    for (int i = 1; i <= so_luong_canh; i++){
                        int node;
                        fscanf(connect, "%d ", &node);
                        char* token;
                        char string[30];

                        fgets(string, 30, connect);
                        token = strtok(string, " ");
                        while(token != NULL){
                            addEdge1(ID_tree, node, atoi(token), 1);
                            addEdge1(URL_tree, getVertex_int(ID_tree, node), getVertex_int(ID_tree, atoi(token)), 1);
                            token = strtok(NULL, " ");
                        }
                    }

                    printf("So luong webpages: %d\n", so_luong_dinh);
                    JRB temp;
                    jrb_traverse(temp, ID_tree.vertices){
                        printf("%s %d\n", getVertex(URL_tree, (int)jval_v(temp->val)), jval_i(temp->key));
                    }
                    /*printf("\n");
                    jrb_traverse(temp, URL_tree.vertices){
                        printf("%d %s\n", jval_i(temp->key), (char*)jval_v(temp->val));
                    }

                    printf("\n");
                    jrb_traverse(temp, ID_tree.vertices){
                        printf("%d %d\n", jval_i(temp->key), (int)jval_v(temp->val));
                    }*/
                }
                    break;
                case 2:
                    if (choice){
                        pageRank info[NumberVertex(URL_tree)];
                        int output[100];
                        for (int i = 1; i <= NumberVertex(URL_tree); i++){
                            info[i].key = i;
                            info[i].output = outdegree(URL_tree, i, output);
                            info[i].rank = 0;
                            info[i].rank_temp = 1;
                        }
                        int input[100];
                        int sl_input;
                        for (int i = 1; i <= NumberVertex(URL_tree); i++){
                            sl_input = indegree(URL_tree, i, input);
                            for (int j = 0; j < sl_input; j++){
                                info[i].rank += info[input[j]].rank_temp/info[input[j]].output;
                            }
                        }
                        for (int i = 1; i <= NumberVertex(URL_tree); i++){
                            info[i].rank_temp = info[i].rank;
                        }

                        for (int i = 1; i <= NumberVertex(URL_tree); i++){
                            printf("%s has rank: %.2f\n", getVertex(URL_tree, i), info[i].rank);
                        }
                    }
                    else printf("Ban chua doc file\n");
                    break;
                case 3:
                    if (choice){
                        pageRank info[NumberVertex(URL_tree)];
                        int output[100];
                        for (int i = 1; i <= NumberVertex(URL_tree); i++){
                            info[i].key = i;
                            info[i].output = outdegree(URL_tree, i, output);
                            info[i].rank = 0;
                            info[i].rank_temp = 1;
                        }
                        int times;
                        goal1:
                        printf("Nhap vao so lan: "); scanf("%d", &times);
                        if (times < 10 || times > 50){
                            printf("Nhap khong hop le!\n");
                            goto goal1;
                        }
                        for (int t = 0; t < times; t++){
                            int input[100];
                            int sl_input;
                            for (int i = 1; i <= NumberVertex(URL_tree); i++){
                                sl_input = indegree(URL_tree, i, input);
                                    for (int j = 0; j < sl_input; j++){
                                        info[i].rank += info[input[j]].rank_temp/info[input[j]].output;
                                    }
                            }
                            for (int i = 1; i <= NumberVertex(URL_tree); i++){
                                info[i].rank_temp = info[i].rank;
                            }
                        }


                        for (int i = 1; i <= NumberVertex(URL_tree); i++){
                            printf("%s has rank: %.2f\n", getVertex(URL_tree, i), info[i].rank);
                        }
                    }
                    else printf("Ban chua doc file\n");
                    break;
                case 4:
                    if(flag){
                        int input[100], output[100];
                        int spam[100], not_spam[100];
                        int pos1 = 0, pos2 = 0;
                        JRB temp;
                        for (int i = 1; i <= NumberVertex(URL_tree); i++){
                            if (indegree(URL_tree, i, input) == 0 && outdegree(URL_tree, i,output) != 0){
                                spam[pos1] = i;
                                pos1++;
                            }
                            if (indegree(URL_tree, i, input) != 0 && outdegree(URL_tree, i,output) == 0){
                                not_spam[pos2] = i;
                                pos2++;
                            }
                        }

                        printf("So luong Web chi co lien ket den: %d\n", pos2);
                        if (pos2 != 0){
                            for (int i = 0; i < pos2; i++) printf("%s ", getVertex(URL_tree, not_spam[i]));
                            printf("\n");
                        }
                        printf("So luong Spam: %d\n", pos1);
                        if (pos1 != 0){
                            for (int i = 0; i < pos1; i++) printf("%s ", getVertex(URL_tree, spam[i]));
                            printf("\n");
                        }

                    }
                    else printf("Ban chua doc file\n");
                    break;
                case 5:
                    if(flag){
                        int ID1, ID2;
                        goal:
                        printf("Nhap ID1: "); scanf("%d", &ID1);
                        printf("Nhap ID2: "); scanf("%d", &ID2);
                        int k1 = getVertex_int(ID_tree, ID1);
                        int k2 = getVertex_int(ID_tree, ID2);
                        if (k1 == k2) printf("-1\n");
                        else if (getVertex(URL_tree, k1) == NULL || getVertex(URL_tree, k2) == NULL) printf("-2\n");
                        else{
                            int length, path[100];
                            float dis = dijkstra(URL_tree, k1, k2, path, &length);
                            if (dis == INFINITIVE_VALUE) printf("-3\n");
                            else printf("Distance: %.f\n", dis);
                        }
                    }
                    else printf("Ban chua doc file\n");
                    break;
                case 6: printf("Ban da thoat chuong trinh!\n");
                    break;
            }
        }
    }while(choice != 6);
    fclose(web);
    fclose(connect);
    return 1;
}
