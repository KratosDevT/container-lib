#include <iostream>
#include <queue> // uso std per semplicità dell'esempio
#include <string>

using namespace std;
class A
{
public:
	int priority;
	A() {}
	A(int a) :priority{ a } {}

	bool operator<(const A& other) const
	{
		return priority < other.priority; // < max-heap, > min-heap
	}
};
int main()
{
	cout << "\n=== ESEMPIO 1: NUMERI (MAX-HEAP) ===" << endl;

	// Priority queue di default = MAX-HEAP 
	// Elemento PIÙ GRANDE ha priorità più alta
	priority_queue<int> pq;

	// Inserisco numeri a caso
	pq.push(5);
	cout << "Inserito: 5" << endl;

	pq.push(10);
	cout << "Inserito: 10" << endl;

	pq.push(3);
	cout << "Inserito: 3" << endl;

	pq.push(8);
	cout << "Inserito: 8" << endl;

	cout << "\nOrdine di inserimento: 5, 10, 3, 8" << endl;
	cout << "Ma quando li estraggo..." << endl;

	// Estraggo: esce sempre il PIÙ GRANDE
	cout << "\nEstraggo in ordine di priorità:" << endl;
	while (!pq.empty())
	{
		cout << "  -> " << pq.top() << " (questo era il più grande!)" << endl;
		pq.pop();
	}

	// Output:
	// 10 (più grande)
	// 8
	// 5
	// 3 (più piccolo)


	cout << "\n\n=== ESEMPIO 2: TASK CON PRIORITÀ ===" << endl;

	struct Task
	{
		string nome;
		int priorita; // Più alto = più importante

		// Questo dice come confrontare i task
		bool operator<(const Task& other) const
		{
			return priorita < other.priorita; // Max-heap per priorità
		}
	};

	priority_queue<Task> task_queue;

	// Aggiungo task in ordine casuale
	task_queue.push({ "Fare la spesa", 2 });
	cout << "Aggiunto: Fare la spesa (priorità 2)" << endl;

	task_queue.push({ "BUG CRITICO!", 10 });
	cout << "Aggiunto: BUG CRITICO! (priorità 10)" << endl;

	task_queue.push({ "Rispondere email", 5 });
	cout << "Aggiunto: Rispondere email (priorità 5)" << endl;

	task_queue.push({ "Leggere documentazione", 1 });
	cout << "Aggiunto: Leggere documentazione (priorità 1)" << endl;

	task_queue.push({ "Meeting importante", 7 });
	cout << "Aggiunto: Meeting importante (priorità 7)" << endl;

	cout << "\nEseguo task in ordine di priorità:" << endl;
	int count = 1;
	while (!task_queue.empty())
	{
		Task next = task_queue.top();
		task_queue.pop();
		cout << count++ << ". " << next.nome
			<< " (priorità " << next.priorita << ")" << endl;
	}

	// Output (in ordine di priorità!):
	// 1. BUG CRITICO! (priorità 10)
	// 2. Meeting importante (priorità 7)
	// 3. Rispondere email (priorità 5)
	// 4. Fare la spesa (priorità 2)
	// 5. Leggere documentazione (priorità 1)


	cout << "\n\n=== ESEMPIO 3: MIN-HEAP (trova il più piccolo) ===" << endl;

	// Per avere il PIÙ PICCOLO in cima, uso greater<int>
	priority_queue<int, vector<int>, greater<int>> min_pq;

	min_pq.push(50);
	min_pq.push(10);
	min_pq.push(30);
	min_pq.push(20);

	cout << "Inseriti: 50, 10, 30, 20" << endl;
	cout << "Con MIN-HEAP, esce prima il più piccolo:" << endl;

	while (!min_pq.empty())
	{
		cout << "  -> " << min_pq.top() << endl;
		min_pq.pop();
	}

	// Output:
	// 10 (più piccolo)
	// 20
	// 30
	// 50 (più grande)


	cout << "\n\n=== ESEMPIO 4: TOP-3 NUMERI PIÙ GRANDI ===" << endl;

	vector<int> numeri = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3 };

	cout << "Array: ";
	for (int n : numeri) cout << n << " ";
	cout << endl;

	// Voglio trovare i 3 numeri più grandi
	// Trucco: uso un MIN-HEAP di size 3
	priority_queue<int, vector<int>, greater<int>> top3;

	for (int num : numeri)
	{
		top3.push(num);

		// Se ho più di 3 elementi, rimuovo il più piccolo
		if (top3.size() > 3)
		{
			top3.pop(); // Rimuove il più piccolo
		}
	}

	cout << "\nI 3 numeri più grandi sono:" << endl;
	vector<int> result;
	while (!top3.empty())
	{
		result.push_back(top3.top());
		top3.pop();
	}

	// Li ho in ordine crescente, li stampo al contrario
	for (int i = result.size() - 1; i >= 0; --i)
	{
		cout << "  " << result[i] << endl;
	}

	// Output:
	// 9
	// 6
	// 5


	cout << "\n\n=== ESEMPIO 5: PRONTO SOCCORSO ===" << endl;

	struct Paziente
	{
		string nome;
		int gravita; // 1=lieve, 10=critico

		bool operator<(const Paziente& other) const
		{
			return gravita < other.gravita; // Max-heap per gravità
		}
	};

	priority_queue<Paziente> pronto_soccorso;

	cout << "Arrivo pazienti (in ordine di arrivo):" << endl;
	pronto_soccorso.push({ "Mario (mal di testa)", 2 });
	cout << "1. Mario - mal di testa (gravità 2)" << endl;

	pronto_soccorso.push({ "Luigi (infarto!)", 10 });
	cout << "2. Luigi - INFARTO! (gravità 10)" << endl;

	pronto_soccorso.push({ "Peach (caviglia slogata)", 4 });
	cout << "3. Peach - caviglia slogata (gravità 4)" << endl;

	pronto_soccorso.push({ "Bowser (frattura grave)", 8 });
	cout << "4. Bowser - frattura grave (gravità 8)" << endl;

	pronto_soccorso.push({ "Toad (raffreddore)", 1 });
	cout << "5. Toad - raffreddore (gravità 1)" << endl;

	cout << "\n>>> ATTENZIONE: Non è FIFO! <<<" << endl;
	cout << "Ordine di cura (per gravità):" << endl;

	int pos = 1;
	while (!pronto_soccorso.empty())
	{
		Paziente p = pronto_soccorso.top();
		pronto_soccorso.pop();
		cout << pos++ << ". " << p.nome
			<< " (gravità " << p.gravita << ")" << endl;
	}

	// Output:
	// 1. Luigi - INFARTO! (gravità 10) <- Arrivato secondo ma curato PRIMO!
	// 2. Bowser - frattura (gravità 8)
	// 3. Peach - caviglia (gravità 4)
	// 4. Mario - mal di testa (gravità 2)
	// 5. Toad - raffreddore (gravità 1) <- Arrivato ultimo e curato ULTIMO!


	std::priority_queue<int> min_pq2;
	min_pq2.emplace(10);
	min_pq2.emplace(12);
	min_pq2.emplace(5);
	min_pq2.emplace(45);
	min_pq2.emplace(3);

	std::cout << "*******************" << std::endl;
	std::cout << min_pq2.top() << std::endl;

	return 0;
}