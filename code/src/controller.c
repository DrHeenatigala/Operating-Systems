//PROTOTYPE: controller.c 

#include <stdio.h>
#include <string.h>
#include <strings.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "../include/errors.h"

#define MAX_CHILDREN 100
pid_t children_pids[MAX_CHILDREN];
int children_count = 0;
int next_children = 0;

typedef enum {
    CMD_UNKNOWN = -1,
    CMD_LIST,
    CMD_ADD,
    CMD_DEL,
    CMD_LINK,
    CMD_SWITCH,
    CMD_INFO,
    CMD_ESCI,
    CMD_ADD_BULB,
    CMD_ADD_WINDOW,
    CMD_ADD_FRIDGE,
    CMD_ADD_HUB,
    CMD_ADD_TIMER
} TipoComando;

// Funzione che converte la stringa nel rispettivo enum
TipoComando ottieni_codice_comando(const char *str) {
    if (strcmp(str, "list") == 0)   return CMD_LIST;
    if (strcmp(str, "add") == 0)    return CMD_ADD;
    if (strcmp(str, "del") == 0)    return CMD_DEL;
    if (strcmp(str, "link") == 0)   return CMD_LINK;
    if (strcmp(str, "switch") == 0) return CMD_SWITCH;
    if (strcmp(str, "info") == 0)   return CMD_INFO;
    if (strcmp(str, "esci") == 0)   return CMD_ESCI;
    if (strcmp(str, "bulb") == 0)   return CMD_ADD_BULB; // comando speciale per testare il bulb
    if (strcmp(str, "window") == 0) return CMD_ADD_WINDOW; // comando speciale per testare la finestra
    if (strcmp(str, "fridge") == 0) return CMD_ADD_FRIDGE; // comando speciale per testare il frigorifero
    if (strcmp(str, "hub") == 0)    return CMD_ADD_HUB; // comando speciale per testare l'hub
    if (strcmp(str, "timer") == 0)  return CMD_ADD_TIMER; // comando speciale per testare il timer
    return CMD_UNKNOWN;
}

void handle_signal(int sig) {
    if(sig == SIGINT || sig == SIGTERM) {
        for (int i = 0; i < children_count; i++) {
            if (children_pids[i] > 0) {
                kill(children_pids[i], SIGTERM);
            }
        }
    }
}

int list_command(const char *arg) {
    if(arg != NULL) {
        fprintf(stderr, "Errore: comando 'list' non accetta argomenti\n");
        return INVALID_COMMAND;
    }
    printf("comando list eseguito\n"); //TODO: implementare la logica di visualizzazione della lista dei dispositivi
    return ERR_OK;
}

int add_command(const char *device, const char *argchk) {
    if(device == NULL) {
        fprintf(stderr, "Errore: comando 'add' richiede un sottocomando (bulb, window, fridge, hub, timer)\n");
        return INVALID_COMMAND;
    }
    if(argchk != NULL) {
        fprintf(stderr, "Errore: comando 'add' accetta solo un sottocomando (bulb, window, fridge, hub, timer)\n");
        return INVALID_COMMAND;
    }
    if(next_children >= MAX_CHILDREN) {
        fprintf(stderr, "Errore: limite massimo di dispositivi raggiunto\n");
        return INVALID_COMMAND;
    }
    pid_t new_device_pid;
    TipoComando device_cmd = ottieni_codice_comando(device);
    switch(device_cmd) {
        case CMD_ADD_BULB:
            printf("comando add bulb eseguito\n"); //TODO: implementare la logica di aggiunta del dispositivo bulb
            fflush(stdout);
            next_children++; // Incrementa il contatore dei figli prima di passarlo al processo timer
            new_device_pid = fork();
            if(new_device_pid == 0) {
                execv("./bulb", (char *[]){"code/bulb", tostring(next_children), NULL}); //TODO: passare gli argomenti necessari al processo bulb
                perror("execv failed");
                exit(EXIT_FAILURE);
            } else if(new_device_pid < 0) {
                perror("fork failed");
                return LINK_FAILED;
            }
            children_pids[children_count++] = new_device_pid;
            return ERR_OK;
        case CMD_ADD_WINDOW:
            printf("comando add window eseguito\n"); //TODO: implementare la logica di aggiunta del dispositivo window
            fflush(stdout);
            next_children++; // Incrementa il contatore dei figli prima di passarlo al processo timer
            new_device_pid = fork();
            if(new_device_pid == 0) {
                children_pids[children_count++] = getpid();
                execv("./window", (char *[]){"code/window", tostring(next_children), NULL}); //TODO: passare gli argomenti necessari al processo window
                perror("execv failed");
                exit(EXIT_FAILURE);
            } else if(new_device_pid < 0) {
                perror("fork failed");
                return LINK_FAILED;
            }
            children_pids[children_count++] = new_device_pid;
            return ERR_OK;
        case CMD_ADD_FRIDGE:
            printf("comando add fridge eseguito\n"); //TODO: implementare la logica di aggiunta del dispositivo fridge
            fflush(stdout);
            next_children++; // Incrementa il contatore dei figli prima di passarlo al processo timer
            new_device_pid = fork();
            if(new_device_pid == 0) {
                children_pids[children_count++] = getpid();
                execv("./fridge", (char *[]){"code/fridge", tostring(next_children), NULL}); //TODO: passare gli argomenti necessari al processo fridge
                perror("execv failed");
                exit(EXIT_FAILURE);
            } else if(new_device_pid < 0) {
                perror("fork failed");
                return LINK_FAILED;
            }
            children_pids[children_count++] = new_device_pid;
            return ERR_OK;
        case CMD_ADD_HUB:
            printf("comando add hub eseguito\n");
            fflush(stdout);
            next_children++; // Incrementa il contatore dei figli prima di passarlo al processo hub
            new_device_pid = fork();
            if(new_device_pid == 0) {
                execv("./hub", (char *[]){"code/hub", tostring(next_children), NULL}); //TODO: creare l'hub
                perror("execv failed");
                exit(EXIT_FAILURE);
            } else if(new_device_pid < 0) {
                perror("fork failed");
                return LINK_FAILED;
            }
            children_pids[children_count++] = new_device_pid;
            return ERR_OK;
        case CMD_ADD_TIMER:
            printf("comando add timer eseguito\n"); //TODO: implementare la logica di aggiunta del dispositivo timer
            fflush(stdout);
            next_children++; // Incrementa il contatore dei figli prima di passarlo al processo timer
            new_device_pid = fork();
            if(new_device_pid == 0) {
                // Child process
                execv("./timer", (char *[]){"code/timer", tostring(next_children), NULL}); //TODO: passare gli argomenti necessari al processo timer
                perror("execv failed");
                exit(EXIT_FAILURE);
            } else if(new_device_pid < 0) {
                perror("fork failed");
                return LINK_FAILED;
            }
            children_pids[children_count++] = new_device_pid;
            return ERR_OK;
        default:
            fprintf(stderr, "sottocomando sconosciuto per 'add': %s\n", device);
            return UNKNOWN_CMD_ERR;
    }
}

int del_command(const char *id, const char *argchk) {
    char *endstrtol;
    if(id == NULL) {
        fprintf(stderr, "Errore: comando 'del' richiede che venga specificato un id\n");
        return INVALID_COMMAND;
    }
    if(argchk != NULL) {
        fprintf(stderr, "Errore: comando 'del' accetta solo un argomento\n");
        return INVALID_COMMAND;
    }
    if(strtol(id, &endstrtol, 10) < 0 || *endstrtol != '\0') {
        fprintf(stderr, "Errore: l'id specificato non è valido\n");
        return INVALID_COMMAND;
    }
    printf("comando del eseguito con argomento: %s\n", id); //TODO: implementare la logica di eliminazione del dispositivo con l'id specificato

    return ERR_OK;
}

int link_command(const char *id1, const char *to, const char *id2) {
    char *endstrtol;
    if(id1 == NULL || to == NULL || id2 == NULL) {
        fprintf(stderr, "Errore: comando 'link' richiede tre argomenti\n");
        return INVALID_COMMAND;
    }
    if(strcmp(to, "to") != 0) {
        fprintf(stderr, "Errore: comando 'link' richiede la parola chiave 'to' come secondo argomento\n");
        return INVALID_COMMAND;
    }
    if(strtol(id1, &endstrtol, 10) < 0 || *endstrtol != '\0') {
        fprintf(stderr, "Errore: l'id1 specificato non è valido\n");
        return INVALID_COMMAND;
    }
    //id2 deve essere un id appartenente ad un hub, controller o timer
    if(strtol(id2, &endstrtol, 10) < 0 || *endstrtol != '\0') {
        fprintf(stderr, "Errore: l'id2 specificato non è valido\n");
        return INVALID_COMMAND;
    }
    printf("comando link eseguito con argomenti: %s %s %s\n", id1, to, id2); //TODO: implementare la logica di collegamento tra i dispositivi con gli id specificati
    return ERR_OK;
}

int cmd_switch(const char *id, const char *label, const char *pos) {
    char *endstrtol;
    if(id == NULL || label == NULL || pos == NULL) {
        fprintf(stderr, "Errore: comando 'switch' richiede tre argomenti\n");
        return INVALID_COMMAND;
    }
    if(strtol(id, &endstrtol, 10) < 0 || *endstrtol != '\0') {
        fprintf(stderr, "Errore: l'id specificato non è valido\n");
        return INVALID_COMMAND;
    }
    if(strcasecmp(pos, "on") != 0 && strcasecmp(pos, "off") != 0) {
        fprintf(stderr, "Errore: il terzo argomento deve essere 'on' o 'off'\n");
        return INVALID_COMMAND;
    }
    printf("comando switch eseguito con argomenti: %s %s %s\n", id, label, pos); //TODO: implementare la logica di cambio stato del dispositivo con l'id specificato, per l'etichetta e la posizione specificate
    return ERR_OK;
    // ok, qua è un casino, dobbiamo prima implementare la comunicazione inter process (in poche parole una tumorata)
    // cose che deve fare il comando switch:
    // 1. controllare che l'id sia valido (cioè che esista un dispositivo con quell'id)
    // 2. controllare la tipologia del dispositivo con quell'id (cioè se è un bulb, una finestra, un frigorifero, un hub o un timer)
    // 3. se l'id appartiene ad un bulb il label deve essere "power"
    // 4. se l'id appartiene ad una finestra il label deve essere "open" o "close"
    // 5. se l'id appartiene ad un frigorifero il label deve essere "open" o "close"
    // 6. se l'id appartiene ad un hub o un timer devono rispecchiare le label dei figli collegati (cioè se un figlio è un bulb il label deve essere "power", se un figlio è una finestra il label deve essere "open" o "close", se un figlio è un frigorifero il label deve essere "open" o "close")
    // 7. se l'id appartiene al controller il label deve essere "main"
}

int cmd_info(const char *id, const char *argchk) {
    char *endstrtol;
    if(id == NULL) {
        fprintf(stderr, "Errore: comando 'info' richiede che venga specificato un id\n");
        return INVALID_COMMAND;
    }
    if(argchk != NULL) {
        fprintf(stderr, "Errore: comando 'info' accetta solo un argomento\n");
        return INVALID_COMMAND;
    }
    if(strtol(id, &endstrtol, 10) < 0 || *endstrtol != '\0') {
        fprintf(stderr, "Errore: l'id specificato non è valido\n");
        return INVALID_COMMAND;
    }
    printf("comando info eseguito con argomento: %s\n", id); //TODO: implementare la logica di visualizzazione delle informazioni del dispositivo con l'id specificato
    return ERR_OK;
}

int main(void){
    // il Controller apre una piccola shell con sei comandi: list, add, del, link, switch, info
    char input[128];
    int error_code = ERR_OK;
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Controller avviato. Inserisci un comando (list, add, del, link, switch, info, esci): \n");
    while(1){
        if(fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0';
        char *keyword = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");
        char *arg3 = strtok(NULL, " ");
        char *argchk = strtok(NULL, " ");

        if (keyword == NULL) {
            continue; // Evita crash se l'utente preme solo Invio
        }
        if (argchk != NULL) {
            fprintf(stderr, "Errore: troppi argomenti forniti\n");
            continue;
        }

        TipoComando cmd = ottieni_codice_comando(keyword);
        switch(cmd) {
            case CMD_LIST:
                error_code = list_command(arg1);
                break;
            case CMD_ADD:
                error_code = add_command(arg1, arg2);
                break;
            case CMD_DEL:
                error_code = del_command(arg1, arg2);
                break;
            case CMD_LINK:
                error_code = link_command(arg1, arg2, arg3);
                break;
            case CMD_SWITCH:
                error_code = cmd_switch(arg1, arg2, arg3);
                break;
            case CMD_INFO:
                error_code = cmd_info(arg1, arg2);
                break;
            case CMD_ESCI:
                printf("Uscita dal controller.\n");
                for (int i = 0; i < children_count; i++) {
                    if (children_pids[i] > 0) {
                        kill(children_pids[i], SIGTERM);
                    }
                }
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "comando sconosciuto: %s\n", keyword);
                printf("Reinserisci un comando valido (list, add, del, link, switch, info, esci): \n");
                error_code = UNKNOWN_CMD_ERR;
        }
    }
}