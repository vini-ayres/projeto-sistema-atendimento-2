#include <iostream>
#include <locale.h>
#include <string>
#include <vector>
using namespace std;

struct No {
    int dado;
    No *prox;
};

struct Fila {
    No *ini;
    No *fim;
};

Fila* init() {
    Fila *f = new Fila;
    f->ini = NULL;
    f->fim = NULL;
    return f;
}

int isEmpty(Fila *f) {
    return (f->ini == NULL);
}

void enqueue(Fila *f, int v) {
    No *no = new No;
    no->dado = v;
    no->prox = NULL;
    if (isEmpty(f)) {
        f->ini = no;
    }
    else {
        f->fim->prox = no;
    }
    f->fim = no;
}

int dequeue(Fila *f) {
    int ret;
    if (isEmpty(f)) {
        ret = -1;
    }
    else {
        No *no = f->ini;
        ret = no->dado;
        f->ini = no->prox;
        if (f->ini == NULL) {
            f->fim = NULL;
        }
        delete no;
    }
    return ret;
}

int count(Fila *f) {
    int qtde = 0;
    No *no;
    no = f->ini;
    while (no != NULL) {
        qtde++;
        no = no->prox;
    }
    return qtde;
}

void print(Fila *f) {
    No *no;
    no = f->ini;
    cout << "-- Qtde de elementos: " << count(f) << endl;
    while (no != NULL) {
        cout << no->dado << endl;
        no = no->prox;
    }
}

void freeFila(Fila *f) {
    No *no = f->ini;
    while (no != NULL) {
        No *temp = no->prox;
        delete no;
        no = temp;
    }
    delete f;
}

struct Guiche {
    int id;
    Fila *senhasAtendidas;
};

vector<Guiche> guiches;

Guiche* findGuiche(int id) {
    for (size_t i = 0; i < guiches.size(); i++) {
        if (guiches[i].id == id) {
            return &guiches[i];
        }
    }
    return nullptr;
}

void abrirGuiche(int id) {
    Guiche guiche;
    guiche.id = id;
    guiche.senhasAtendidas = init();
    guiches.push_back(guiche);
    cout << "Guichê " << id << " aberto." << endl;
}

void listarSenhasAtendidas(int id) {
    Guiche *guiche = findGuiche(id);
    if (guiche != nullptr) {
        cout << "Senhas atendidas pelo guichê " << id << ":" << endl;
        print(guiche->senhasAtendidas);
    } else {
        cout << "Guichê " << id << " não encontrado." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "");
    Fila *senhasGeradas = init();
    int opcao;
    int senha = 0;

    do {
        string line(40, '-');
        cout << line << endl;
        cout << "0. Sair" << endl;
        cout << "1. Gerar senha" << endl;
        cout << "2. Abrir guichê" << endl;
        cout << "3. Realizar atendimento" << endl;
        cout << "4. Listar senhas atendidas" << endl;
        cout << "Opção: ";
        cin >> opcao;

        if (opcao == 0 && !isEmpty(senhasGeradas)) {
            cout << "Ainda há senhas aguardando atendimento. Não é possível sair." << endl;
            opcao = -1;
        }

        switch (opcao) {
            case 0:
                cout << line << endl;
                cout << "Encerrando programa...\n";
                int totalAtendidas = 0;
                for (const auto& guiche : guiches) {
                    totalAtendidas += count(guiche.senhasAtendidas);
                }
                cout << "Total de senhas atendidas: " << totalAtendidas << endl;
                break;
            case 1:
                cout << line << endl;
                senha++;
                enqueue(senhasGeradas, senha);
                cout << "Senha gerada: " << senha << endl;
                cout << "Senhas aguardando atendimento: " << count(senhasGeradas) << endl;
                break;
            case 2: {
                cout << line << endl;
                int idGuiche;
                cout << "Digite o ID do guichê a ser aberto: ";
                cin >> idGuiche;
                abrirGuiche(idGuiche);
                cout << "Senhas aguardando atendimento: " << count(senhasGeradas) << endl;
                cout << "Número de guichês abertos: " << guiches.size() << endl;
                break;
            }
            case 3: {
                cout << line << endl;
                if (isEmpty(senhasGeradas)) {
                    cout << "Nenhuma senha para atender." << endl;
                    break;
                }
                int idGuiche;
                cout << "Digite o ID do guichê: ";
                cin >> idGuiche;
                Guiche *guiche = findGuiche(idGuiche);
                if (guiche != nullptr) {
                    int senhaAtendida = dequeue(senhasGeradas);
                    enqueue(guiche->senhasAtendidas, senhaAtendida);
                    cout << "Senha atendida: " << senhaAtendida << endl;
                } else {
                    cout << "Guichê " << idGuiche << " não encontrado." << endl;
                }
                cout << "Senhas aguardando atendimento: " << count(senhasGeradas) << endl;
                break;
            }
            case 4: {
                cout << line << endl;
                int idGuiche;
                cout << "Digite o ID do guichê: ";
                cin >> idGuiche;
                listarSenhasAtendidas(idGuiche);
                break;
            }
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }

    } while (opcao != 0);

    freeFila(senhasGeradas);
    for (auto& guiche : guiches) {
        freeFila(guiche.senhasAtendidas);
    }

    return 0;
}
