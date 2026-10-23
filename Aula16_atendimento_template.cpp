// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  DESAFIO SQUAD — Sistema de Atendimento                                 ║
// ║  PSC II · Engenharia de Software · Campo Real · Prof. Giovane Galvão    ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  Compilar:  g++ -std=c++17 -Wall -o atendimento atendimento_template.cpp ║
// ║  Rodar:     ./atendimento                                                ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  REGRAS:                                                                 ║
// ║   • Não usar STL (vector, queue, stack, list etc.)                       ║
// ║   • Toda alocação dinâmica (new) deve ter seu delete correspondente      ║
// ║   • Commitar a cada requisito implementado (R1, R2, R3, R4)             ║
// ║   • README.md no repositório explicando como compilar e usar             ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  RANKING:                                                                ║
// ║   +1 por requisito que compila e funciona                                ║
// ║   +1 por commit organizado com mensagem descritiva                       ║
// ║   +2 por README.md completo                                              ║
// ║   +2 squad com melhor demo (2 min)                                       ║
// ╚══════════════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <string>
using namespace std;

struct No {
    string nome;
    int    urgencia;   
    No*    prox;
    No(const string& n, int u = 0) : nome(n), urgencia(u), prox(nullptr) {}
};


struct FilaPacientes {
    No* frente = nullptr;
    No* tras   = nullptr;

    void enqueue(const string& nome, int urgencia) {
        No* novo = new No(nome, urgencia);
        if (isEmpty()) {
            frente = novo;
            tras   = novo;
        } else {
            tras->prox = novo;
            tras = novo;
        }
    }

    string dequeue() {
        if (isEmpty()) return "";
        No* removido = frente;
        string nome = removido->nome;
        frente = frente->prox;
        if (frente == nullptr) tras = nullptr;
        delete removido;
        return nome;
    }

    string frente_peek() const {
        if (isEmpty()) return "";
        return frente->nome;
    }

    bool isEmpty() const { return frente == nullptr; }

    int tamanho() const {
        int n = 0;
        for (No* p = frente; p != nullptr; p = p->prox) n++;
        return n;
    }

    ~FilaPacientes() {
        while (!isEmpty()) dequeue();
    }
};


struct PilhaHistorico {
    No* topo = nullptr;

    void push(const string& nome) {
        No* novo = new No(nome);
        novo->prox = topo;
        topo = novo;
    }

    string pop() {
        if (isEmpty()) return "";
        No* removido = topo;
        string nome = removido->nome;
        topo = topo->prox;
        delete removido;
        return nome;
    }

    string peek() const {
        if (isEmpty()) return "";
        return topo->nome;
    }

    bool isEmpty() const { return topo == nullptr; }

    ~PilhaHistorico() {
        while (!isEmpty()) pop();
    }
};

struct ListaMedicos {
    No* head = nullptr;

    void adicionarMedico(const string& nome) {
        No* novo = new No(nome);
        if (head == nullptr) {
            head = novo;
            return;
        }
        No* p = head;
        while (p->prox != nullptr) p = p->prox;
        p->prox = novo;
    }

    string chamarMedico() {
        if (head == nullptr) return "";
        No* removido = head;
        string nome = removido->nome;
        head = head->prox;
        delete removido;
        return nome;
    }

    void percorrer(const string& prefixo = "") const {
        if (!prefixo.empty()) cout << prefixo << ": ";
        if (head == nullptr) {
            cout << "(lista vazia)\n";
            return;
        }
        No* p = head;
        while (p) {
            cout << p->nome;
            if (p->prox) cout << " -> ";
            p = p->prox;
        }
        cout << "\n";
    }

    bool temMedico() const { return head != nullptr; }

    ~ListaMedicos() {
        while (head) {
            No* t = head;
            head  = head->prox;
            delete t;
        }
    }
};


void imprimirHistorico(PilhaHistorico& hist) {
    cout << "\n=== Relatório de Atendimentos ===\n";
    if (hist.isEmpty()) {
        cout << "(nenhum atendimento registrado)\n";
        return;
    }
    PilhaHistorico aux;
    int i = 1;
    while (!hist.isEmpty()) {
        string item = hist.pop();
        cout << "  " << i++ << ". " << item << "\n";
        aux.push(item);
    }
    while (!aux.isEmpty()) {
        hist.push(aux.pop());
    }
}


int main() {
    cout << "╔═══════════════════════════════════════════════════╗\n";
    cout << "║  Sistema de Atendimento — Squad ________________  ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n\n";

    FilaPacientes  fila;
    PilhaHistorico historico;
    ListaMedicos   medicos;

    medicos.adicionarMedico("Dra. Ana Lima");
    medicos.adicionarMedico("Dr. Bruno Costa");
    medicos.adicionarMedico("Dra. Carla Ramos");
    medicos.percorrer("Médicos disponíveis");

    cout << "\nPacientes chegando:\n";
    fila.enqueue("Eduardo (urgencia=3)", 3);
    fila.enqueue("Fernanda (urgencia=1)", 1);
    fila.enqueue("Gabriel (urgencia=2)", 2);
    fila.enqueue("Helena (urgencia=1)", 1);

    cout << "Próximo a ser atendido: " << fila.frente_peek() << "\n";
    cout << "Total na fila: " << fila.tamanho() << "\n";

    cout << "\nSimulação de atendimentos:\n";
    int atend = 1;
    while (!fila.isEmpty() && medicos.temMedico()) {
        string paciente = fila.dequeue();
        string medico   = medicos.chamarMedico();

        cout << "  [" << atend++ << "] " << medico
             << " atende " << paciente << "\n";

        historico.push(medico + " → " + paciente);

        medicos.adicionarMedico(medico);
    }

    cout << "\nÚltimo atendimento registrado (peek): " << historico.peek() << "\n";

    imprimirHistorico(historico);

    return 0;
}

// ════════════════════════════════════════════════════════════════
// CHECKLIST DE ENTREGA
// ════════════════════════════════════════════════════════════════
// [ ] TODO 1 — enqueue funciona (paciente entra na fila)
// [ ] TODO 2 — dequeue funciona (paciente sai da fila, memória liberada)
// [ ] TODO 3 — frente_peek retorna o nome sem remover
// [ ] TODO 4 — push empilha no histórico
// [ ] TODO 5 — pop desempilha e libera memória
// [ ] TODO 6 — peek retorna o topo sem remover
// [ ] TODO 7 — adicionarMedico insere no final da lista
// [ ] TODO 8 — chamarMedico remove do início e retorna o nome
// [ ] TODO 9 — percorrer imprime todos os médicos com setas
// [ ] TODO 10 — imprimirHistorico imprime sem destruir os dados
// [ ] Programa compila sem warnings: g++ -std=c++17 -Wall -o sis main.cpp
// [ ] Sem vazamentos de memória (todo new tem delete correspondente)
// [ ] Repositório no GitHub com histórico de commits organizado
// [ ] README.md com instrução de compilação e exemplo de saída