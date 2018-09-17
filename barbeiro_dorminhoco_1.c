/*Neste código temos um problema de sincronização das threads
pois as vezes indica que não tem cadeira disponível para tal cliente 
mesmo que um outro cliente tenha acabado de cortar o cabelo*/



#include<cstdio>
#include<cstdlib>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

int n_clientes = 15; // 15 clientes que vão vim

//Declaração dos semaforos
sem_t sem_cadeira;					//semaforo cadeiras livras
sem_t sem_cadeira_atendimento;		//semaforo cadeira do barbeiro
sem_t sem_cabelo_cortado;			//semaforo para cabelo cortado ou nao

pthread_t barbeiro;					//thread para barbeiro
pthread_t cliente[15];				//thread para clientes
int id[15];

void * vida_cliente(void *var){ //recebe o identificador do cliente em que estamos tratando 
	int id = *(int*) var;		//faz o recebimento do cliente que estamos tratando
	usleep(id*2);				//dorme um pouco, só por uma questão de sincronização
	if(sem_trywait(&sem_cadeira) == 0){					//se tiver cadeiras livres ela será ocupada
		printf("Tem cadeiras para o cliente %d.\n", id);
		sem_wait(&sem_cadeira_atendimento);				//aguarda a cadeira do barbeiro ficar livre
		sem_post(&sem_cadeira);							//sinaliza que está sendo atendido
		sem_wait(&sem_cabelo_cortado);					//espera que o barbeiro sinalize que o cabelo foi cortado
		printf("O cliente %d cortou o cabelo.\n\n", id);
		sem_post(&sem_cadeira);							//libera a cadeira
		sem_post(&sem_cadeira_atendimento);				//libera o barbeiro	
	}
	else //caso não exista cadeiras vazias o cliente vai embora
		printf("Nao havia cadeiras para o cliente %d\n\n.", id);
		pthread_exit(NULL);
}

void * vida_barbeiro(void * var){
	usleep(10);											//tempo para organizar o salão
	while(1){
		sem_wait(&sem_cadeira);							//espera ter cliente e dorme enquanto isso
		printf("Barbeiro atendendo cliente\n");
		usleep(500);									//realizando atendimento
		sem_post(&sem_cabelo_cortado);					//terminou de atender ou seja o cabelo foi cortado
	}
	pthread_exit(NULL);
}

int main(){
	sem_init(&sem_cadeira, 0, 3);						//inicia o semaforo
	sem_init(&sem_cadeira_atendimento, 0, 1);			//inicia o semaforo
	sem_init(&sem_cabelo_cortado, 0, 0);				//inicia o semaforo
	int j =0;
	pthread_create(&barbeiro, NULL, &vida_barbeiro,(void *) &id[j]); //cria thread do barbeiro
	
	//criação de todos os clientes
	for(j=0; j < n_clientes; j++){
		id[j] = j;
		pthread_create(&cliente[j], NULL, &vida_cliente,(void *) &id[j]); // cria thread cliente
		usleep(rand()%100);		//tempo para a criação de cada cliente 				
	}
	
	//recebe a saída de cada uma das threads dos clientes
	for(int i = 0; i < n_clientes; i++)
		pthread_join(cliente[i], NULL);
		
	printf("Acabou o espediente e barbeiro foi para casa dormir\n");
	
	return 0;
}




//Adaptado de: https://www.youtube.com/watch?v=6PH-M7qyRLo&t=91s