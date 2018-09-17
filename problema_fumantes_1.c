#include  <pthread.h>
#include  <semaphore.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <time.h>

pthread_t fumantes[3]; //0 para o papel, 1 para o tabaco e 2 para os fosforos
				
pthread_t agente; //thread para o agente

pthread_mutex_t mutex_mesa; //região crítica, onde os fumantes vão pegar os ingredientes (mesa)

sem_t sem_terminou_fumar; //semaforo para sinalizar que o fumante terminou de fumar

int ingrediente[3];

void fumar(int j){
	printf("Fumante %d esta fumando.\n", j); //Mostra qual fumante está fumando
	pthread_mutex_lock(&mutex_mesa);		//bloqueia o acesso a mesa para nenhum fumante tente pegar os ingredientes 
	int i;
	for(int i = 0; i < 3; i++)
		ingrediente[i] = 0;						//zera todos os ingredientes
	sleep(rand()%3+1);						//tempo para fumar
	printf("Fumante %d terminou de fumar. \n\n", j); //avisa que ele terminou de fumar
	pthread_mutex_unlock(&mutex_mesa);		//libera o acesso a mesa
	sem_post(&sem_terminou_fumar);			//sinaliza o semaforo avisando que ele parou de fumar
	return;	
}

void *f_agente(void * k){
	int var1, var2;
	while(1){
		sleep(rand()%3+1);					//tempo para colocar os ingredientes na mesa
		var1 = rand()%3;					//decide quais ingredientes serão colocados na mesa
		var2 = rand()%3;
		
		while(var2 == var1){ 	//verifica se são iguais, pois o agente tem que escolher dois ingredientes diferentes 
			var2 = rand()%3;
		}
		printf("Agente colocou na mesa: ");  				//mostra o que colocou na mesa
		if(var1 == 0 || var2 == 0)
			printf(" papel ");
		if(var1 == 1 || var2 == 1)
			printf(" tabaco ");
		if(var1 == 2 || var2 == 2)
			printf(" fosforos ");
		printf("\n\n");
		
		ingrediente[var2] = 1;				
		ingrediente[var1] = 1;
		
		//Espera um semaforo que o fumante vai sinalizar, para confirmar que ele terminou de fumar e assim o agente poderá colocar na mesa os igredientes de novo
		sem_wait(&sem_terminou_fumar);		//espera o fumante terminar de fumar		
	}
}

void *fumante_0(void * k){
	usleep(500);			//fumante esta indo ate o agente
	int i = 0;					//identificador do fumante
	while(1){
		if(ingrediente[1] == 1 && ingrediente[2] == 1){		//verifica se tem os produtos que ele quer
			fumar(i);			//se tiver, fuma
		}
	}
}

void *fumante_1(void * k){
	usleep(500);			//fumante esta indo ate o agente
	int i = 1;					//identificador do fumante
	while(1){								
		if(ingrediente[0] == 1 && ingrediente[2] == 1){		//verifica se tem os produtos que ele quer
			fumar(i);		//se tiver, fuma
		}
	}
}

void *fumante_2(void * k){
	usleep(500);			//fumante esta indo ate o agente
	int i = 2;					//identificador do fumante
	while(1){
		if(ingrediente[1] == 1 && ingrediente[0] == 1){		//verifica se tem os produtos que ele quer
			fumar(i);			//se tiver, fuma
		}
	}
}

int main(){
	int j;
	printf("Fumante 0 tem papel,\nFumante 1 tem tabaco,\nFumante 2 tem fosforos.\n\n"); //fala o que cada fumante tem
	srand(time(NULL)); 							
	for(int i = 0; i < 3; i++)					//zera a qtde de ingredientes 
		ingrediente[i] = 0;
	sem_init(&sem_terminou_fumar, 1, 0);		//inicia semaforo
	
	pthread_create(&fumantes[j], NULL, &fumante_0, NULL);	//cria a thread para o fumante 0
	pthread_create(&fumantes[j], NULL, &fumante_1, NULL);	//cria a thread para o fumante 1
	pthread_create(&fumantes[j], NULL, &fumante_2, NULL);	//cria a thread para o fumante 2
	
	pthread_create(&agente, NULL, &f_agente, NULL);	//cria a thread para o agente 
	
	while(1){
		//repetição do ciclo
	}
	
	return 0;	
}

//Adaptado de: https://www.youtube.com/watch?v=fpTiTUSFWSA