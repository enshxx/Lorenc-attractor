CC = g++ -O3
CFLAGS = -std=c++11

# Имена исполняемых файлов
EXEC_LORENZ = lorenz

# Начальные значения
X = 3.051522 
Y = 1.582542
Z = 15.62388 

# Файлы для очистки
CLEAN_FILES = attractor_data.txt research_data.txt sequence.txt fractal.txt

# Правило для вывода помощи
.PHONY: help
help:
	@echo "make attractor_plot - построить график аттрактора"
	@echo "make research_plot - построить график исследования бинарной последовательности"
	@echo "make clean - удалить вспомогательные файлы"
	@echo "Указать начальные условия можно прописав X=<> Y=<> Z=<> после команды"

# Правило для сборки attractor_plot
attractor_plot: clean
	$(CC) $(CFLAGS) lorenz.cpp -o $(EXEC_LORENZ)
	./lorenz $(X) $(Y) $(Z)
	gnuplot attractor.plot

# Правило для сборки research_plot
research_plot: clean
	$(CC) $(CFLAGS) lorenz.cpp -o $(EXEC_LORENZ)
	./lorenz $(X) $(Y) $(Z)
	gnuplot sequence_research.plot

fractal_plot: clean
	$(CC) $(CFLAGS) lorenz.cpp -o $(EXEC_LORENZ)
	./lorenz $(X) $(Y) $(Z)
	gnuplot fractal.plot
# Правило для очистки
clean:
	rm -f $(CLEAN_FILES) $(EXEC_LORENZ)


