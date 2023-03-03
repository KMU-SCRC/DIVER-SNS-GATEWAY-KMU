//#define CURL_STATICLIB
//#pragma comment (lib, "wldap32.lib")
//#pragma comment (lib, "ws2_32.lib")
//#pragma comment(lib, "CRYPT32.lib")
//#pragma comment(lib, "Normaliz.lib")

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(void)
{
    CURL* curl_get;
    CURL* curl_post;
    CURLcode res;
    int quit = 0;
    int init = 0;
    int gnum = 0;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    struct curl_slist* headerlist = NULL;
    headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

    char name[100] = "test1";
    char date[100] = "2023";
    char acceleration[10] = "0.1";
    char gaussMagnetic[10] = "0.1";
    char angularRate[10] = "0.1";
    char underwaterMicrophone[10] = "0.1";
    char gesture[10] = "0.1";
    char proximity[10] = "0.1";
    char dryAmbientLight[10] = "0.1";
    char dryAmbientRGB[10] = "0.1";
    char dryBarometricPressure[10] = "0.1";
    char dryTemperature[10] = "0.1";
    char dryHumidity[10] = "0.1";
    char underwaterPressure[10] = "0.1";
    char underwaterTemperature[10] = "0.1";
    char underwaterDepth[10] = "0.1";
    char seaLevelAltitude[10] = "0.1";
    char underwaterPictureLow[10] = "0.1";
    char underwaterPictureHigh[10] = "0.1";
    char underwaterVideoWithoutMic[10] = "0.1";
    char underwaterVideoWithMic[10] = "0.1";
    char gnss[10] = "0.1";

    char postthis[1000] = "{\"name\": \"test1\", \"date\": \"2023\", \"acceleration\": \"0.1\", \"gaussMagnetic\": \"0.1\", \"angularRate\": \"0.1\", \"underwaterMicrophone\": \"0.1\", \"gesture\": \"0.1\", \"proximity\": \"0.1\", \"dryAmbientLight\": \"0.1\", \"dryAmbientRGB\": \"0.1\", \"dryBarometricPressure\": \"0.1\", \"dryTemperature\": \"0.1\", \"dryHumidity\": \"0.1\", \"underwaterPressure\": \"0.1\", \"underwaterTemperature\": \"0.1\", \"underwaterDepth\": \"0.1\", \"seaLevelAltitude\": \"0.1\", \"underwaterPictureLow\": \"0.1\", \"underwaterPictureHigh\": \"0.1\", \"underwaterVideoWithoutMic\": \"0.1\", \"underwaterVideoWithMic\": \"0.1\", \"gnss\": \"0.1\"}";
    char input[100];
    char* temp;
    char chunk_temp[1000];
    char ID[200];
    char* ID_CODE;
    char POST_URL[100];

    curl_get= curl_easy_init();
    curl_post = curl_easy_init();
    if (!curl_get&&!curl_post) {
        quit = 1;
    }
    while (!quit) {

        if (!init) {
            curl_easy_setopt(curl_get, CURLOPT_FOLLOWLOCATION, 1L);
            /* send all data to this function  */
            curl_easy_setopt(curl_get, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            /* we pass our 'chunk' struct to the callback function */
            curl_easy_setopt(curl_get, CURLOPT_WRITEDATA, (void*)&chunk);

            curl_easy_setopt(curl_post, CURLOPT_HTTPHEADER, headerlist);
            curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS, postthis);
            curl_easy_setopt(curl_post, CURLOPT_POST, 1L);
            /* send all data to this function  */
            curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            /* we pass our 'chunk' struct to the callback function */
            curl_easy_setopt(curl_post, CURLOPT_WRITEDATA, (void*)&chunk);

            init = 1;
        }

        while (!quit) {//로그인
#ifdef _WIN32
            printf("선택하세요.\n1.로그인\n2.종료\n");
#else
            printf("MENU\n1.LOGIN\n2.EXIT\n");
#endif
            scanf("%s", input);
            if (strcmp(input, "1") == 0) {
#ifdef _WIN32
                printf("아이디를 입력해주세요.\n");
#else
                printf("ID\n");
#endif
                scanf("%s", input);
                sprintf(ID, "http://52.78.83.63:8080/api/users/user?email=%s",input);

                curl_easy_setopt(curl_get, CURLOPT_URL, ID);

                /* Perform the request, res will get the return code */
                res = curl_easy_perform(curl_get);
                /* Check for errors */
                if (res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

                //printf("청크 : %s\n", chunk.memory);
                sprintf(chunk_temp, "%s", chunk.memory);
                temp = strtok(chunk_temp, " {}\":,");
                //printf("청크 : %s\n청크템프 : %s\n템프 : %s\n", chunk.memory, chunk_temp, temp);
                free(chunk.memory);
                chunk.memory = malloc(1);
                chunk.size = 0;
                if (temp == NULL) {
#ifdef _WIN32
                    printf("존재하지 않는 아이디입니다.\n");
#else
                    printf("INCORRECT ID\n");
#endif
                }
                else if (strcmp(temp, "id") == 0) {
#ifdef _WIN32
                    printf("로그인 성공\n");
#else
                    printf("LOGIN SUCCESS\n");
#endif
                    ID_CODE = strtok(NULL, " {}\":,");
                    sprintf(POST_URL, "http://52.78.83.63:8080/api/underwater/%s", ID_CODE);
                    //printf("유알엘:%s\n",POST_URL);
                    curl_easy_setopt(curl_post, CURLOPT_URL, POST_URL);
                    break;
                }
                else {
#ifdef _WIN32
                    printf("존재하지 않는 아이디입니다.\n");
#else
                    printf("INCORRECT ID\n");
#endif
                }

            }
            else if (strcmp(input, "2") == 0) {
                quit = 1;
#ifdef _WIN32
                printf("종료합니다.\n");
#else
                printf("EXIT\n");
#endif
                break;
            }
            else {
#ifdef _WIN32
                printf("잘 못 된 입력입니다. 다시 입력해주세요.\n");
#else
                printf("INCORRECT NUMBER\n");
#endif
            }
        }

        if (quit) {
            break;
        }

        while (!quit) {//데이터송수신
#ifdef _WIN32
            printf("선택하세요.\n1.로그아웃\n2.시리얼 데이터 수신\n3.수중 데이터 서버 전송\n4.종료\n");
#else
            printf("MENU\n1.LOGOUT\n2.READ DATA\n3.SEND DATA\n4.EXIT\n");
#endif
            scanf("%s", input);
            if (strcmp(input, "1") == 0) {
#ifdef _WIN32
                printf("로그아웃합니다.\n");
#else
                printf("LOGOUT\n");
#endif
                break;
            }
            else if (strcmp(input, "2") == 0) {
#ifdef _WIN32
                printf("데이터를 수신합니다.\n");
#else
                printf("READ DATA\n");
#endif
                sprintf(name, "test2");
                sprintf(date, "2023");
                sprintf(postthis, "{\"name\": \"%s\", \"date\": \"%s\", \"acceleration\": \"%s\", \"gaussMagnetic\": \"%s\", \"angularRate\": \"%s\", \"underwaterMicrophone\": \"%s\", \"gesture\": \"%s\", \"proximity\": \"%s\", \"dryAmbientLight\": \"%s\", \"dryAmbientRGB\": \"%s\", \"dryBarometricPressure\": \"%s\", \"dryTemperature\": \"%s\", \"dryHumidity\": \"%s\", \"underwaterPressure\": \"%s\", \"underwaterTemperature\": \"%s\", \"underwaterDepth\": \"%s\", \"seaLevelAltitude\": \"%s\", \"underwaterPictureLow\": \"%s\", \"underwaterPictureHigh\": \"%s\", \"underwaterVideoWithoutMic\": \"%s\", \"underwaterVideoWithMic\": \"%s\", \"gnss\": \"%s\"}", name, date, acceleration, gaussMagnetic, angularRate, underwaterMicrophone, gesture, proximity, dryAmbientLight, dryAmbientRGB, dryBarometricPressure, dryTemperature, dryHumidity, underwaterPressure, underwaterTemperature, underwaterDepth, seaLevelAltitude, underwaterPictureLow, underwaterPictureHigh, underwaterVideoWithoutMic, underwaterVideoWithMic, gnss);
                //sprintf(postthis, "{\"name\": \"%s\", \"date\": \"%s\"}",name,date);
            }
            else if (strcmp(input, "3") == 0) {
#ifdef _WIN32
                printf("데이터를 송신합니다.\n");
#else
                printf("SEND DATA\n");
#endif
                curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS, postthis);

                /* Perform the request, res will get the return code */
                res = curl_easy_perform(curl_post);
                /* Check for errors */
                if (res != CURLE_OK)
                    fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

                //printf("청크 : %s\n", chunk.memory);
                sprintf(chunk_temp, "%s", chunk.memory);
                //temp = strtok(chunk_temp, " {}\":,");
                //printf("청크 : %s\n청크템프 : %s\n템프 : %s\n", chunk.memory, chunk_temp, temp);
                gnum = atoi(chunk_temp);
                free(chunk.memory);
                chunk.memory = malloc(1);
                chunk.size = 0;
                if (gnum == 0) {
#ifdef _WIN32
                    printf("데이터 전송에 실패했습니다.\n");
#else
                    printf("FAIL\n");
#endif
                }
                else {
#ifdef _WIN32
                    printf("%d번째 수중 데이터 업로드 완료되었습니다.\n", gnum);
#else
                    printf("SUCCESS\n");
#endif
                }

            }
            else if (strcmp(input, "4") == 0) {
                quit = 1;
#ifdef _WIN32
                printf("종료합니다.\n");
#else
                printf("EXIT\n");
#endif
                break;
            }
            else {
#ifdef _WIN32
                printf("잘 못 된 입력입니다. 다시 입력해주세요.\n");
#else
                printf("INCORRECT NUMBER\n");
#endif
            }
        }

        if (quit) {
            break;
        }

    }
    //printf("빠져나옴1\n");
    /* always cleanup */
    curl_easy_cleanup(curl_get);
    //printf("빠져나옴2\n");
    free(chunk.memory);
    //printf("빠져나옴3\n");
    curl_easy_cleanup(curl_post);
    //printf("빠져나옴4\n");
    curl_slist_free_all(headerlist);
    //printf("빠져나옴5\n");
    return 0;
}