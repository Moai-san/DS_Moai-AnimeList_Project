#include <stdio.h>
#include <stdlib.h>
char* sign_up()//registro
{
    char garbageBin;//basurero
    char ruta [31];//ruta
    FILE* user;//archivo
    char* username =((char*)calloc(21,(sizeof(char))));//nombre de usuario
    char* password =((char*)calloc(21,(sizeof(char))));//contraseña
    printf("Inserte Username (maximo 20 caracteres) 'w' \n");
    scanf("%c",&garbageBin);
    scanf("%[^\n]",username);
    snprintf(ruta,sizeof(ruta),"User/%s%s",username,".txt");//se abre archivo con el username ingresado
    user =(fopen(ruta,"r"));
    while(user!=NULL)//de existir el username se re solicita username hasta ingresar uno que no exista
    {
        fclose(user);
        printf("Usuario ya Existe, intente nuevamente!!!\n");
        scanf("%c",&garbageBin);
        scanf("%[^\n]",username);
        snprintf(ruta,sizeof(ruta),"User/%s%s",username,".txt");
        user =(fopen(ruta,"r"));
    }
    printf("Inserte Password (maximo 20 caracteres) 'w' \n");
    scanf("%c",&garbageBin);//se solicita contraseña
    scanf("%[^\n]",password);
    if (password==NULL)//de no ingresar nada se resolicita la contraseña
    {
        printf("Debe insertar una password (maximo 20 caracteres) 'w' \n");
        scanf("%c",&garbageBin);
        scanf("%[^\n]",password);
    }
    user =fopen(ruta,"w");//se crean los datos de usuario
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
    return(username);//se retorna el nombre de usuario
}

char* log_me()//inicio de sesion
{
    char garbageBin;//basurero
    char ruta [31];//ruta archivo
    char* correct_password =((char*)calloc(20,(sizeof(char))));//contraseña
    FILE* user;    //archivo
    short int MAX_ATTEMPTS =(3);//maximos intentos de contraseña
    char* username =((char*)calloc(20,(sizeof(char))));//nombre de usuario
    char* password =((char*)calloc(20,(sizeof(char))));//contraseña ingresada
    printf("Inserte Username 'w' \n");
    scanf("%c",&garbageBin);
    scanf("%[^\n]",username);
    snprintf(ruta,sizeof(ruta),"User/%s%s",username,".txt");
    user =(fopen(ruta,"r"));
    while(user==NULL)//mientras el usuario no exista se le seguira solicitando ingresar uno
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
    while ((strcmp(password,correct_password))!=0)//se daran 4 intentos de insertar la contraseña, al llegar a cero estos, se terminara la ejecucion
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
    return(username);//se retorna el nombre de usuario
}