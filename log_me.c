#include <stdio.h>
#include <stdlib.h>
char* sign_up()
{
    char garbageBin;
    char ruta [31];
    FILE* user;
    char* username =((char*)calloc(21,(sizeof(char))));
    char* password =((char*)calloc(21,(sizeof(char))));
    printf("Inserte Username (maximo 20 caracteres) 'w' \n");
    scanf("%c",&garbageBin);
    scanf("%[^\n]",username);
    snprintf(ruta,sizeof(ruta),"User/%s%s",username,".txt");
    user =(fopen(ruta,"r"));
    while(user!=NULL)
    {
        fclose(user);
        printf("Usuario ya Existe, intente nuevamente!!!\n");
        scanf("%c",&garbageBin);
        scanf("%[^\n]",username);
        snprintf(ruta,sizeof(ruta),"User/%s%s",username,".txt");
        user =(fopen(ruta,"r"));
    }
    printf("Inserte Password (maximo 20 caracteres) 'w' \n");
    scanf("%c",&garbageBin);
    scanf("%[^\n]",password);
    if (password==NULL)
    {
        printf("Debe insertar una password (maximo 20 caracteres) 'w' \n");
        scanf("%c",&garbageBin);
        scanf("%[^\n]",password);
    }
    user =fopen(ruta,"w");
    fprintf(user,"%s",password);
    fclose(user);
    snprintf(ruta,sizeof(ruta),"User/Fav/%s%s",username,".csv");
    user =fopen(ruta,"w");
    fclose(user);
    snprintf(ruta,sizeof(ruta),"User/Hate/%s%s",username,".csv");
    user =fopen(ruta,"w");
    fclose(user);
    snprintf(ruta,sizeof(ruta),"User/Watched/%s%s",username,".csv");
    user =fopen(ruta,"w");
    fclose(user);
    return(username);
}
char* log_me()
{
    char garbageBin;
    char ruta [31];
    char* correct_password =((char*)calloc(20,(sizeof(char))));
    FILE* user;    
    short int MAX_ATTEMPTS =(3);
    char* username =((char*)calloc(20,(sizeof(char))));
    char* password =((char*)calloc(20,(sizeof(char))));
    printf("Inserte Username 'w' \n");
    scanf("%c",&garbageBin);
    scanf("%[^\n]",username);
    snprintf(ruta,sizeof(ruta),"User/%s%s",username,".txt");
    user =(fopen(ruta,"r"));
    while(user==NULL)
    {
        printf("Usuario no Existe, intente nuevamente!!!\n");
        scanf("%c",&garbageBin);
        scanf("%[^\n]",username);
        snprintf(ruta,sizeof(ruta),"User/%s%s",username,".txt");
        user =(fopen(ruta,"r"));
    }
    printf("Inserte Password 'w' \n");
    scanf("%c",&garbageBin);
    scanf("%[^\n]",password);
    fscanf (user,"%s",correct_password);
    while ((strcmp(password,correct_password))!=0)
    {
        if (MAX_ATTEMPTS==0)
        {
            printf("Acceso Ilegal, Adios!\n");
            exit(1);
        }
        printf("Contraseña invalida!!!\nIntente nuevamente!!\n%d intentos restantes!\n",MAX_ATTEMPTS);
        scanf("%c",&garbageBin);
        scanf("%[^\n]",password);
        MAX_ATTEMPTS =(MAX_ATTEMPTS-1);
    }
    fclose(user);
    return(username);
}