CC = g++ -O3
CFLAGS = -std=c++11

# Имена исполняемых файлов
EXEC_LORENZ = lorenz

# Начальные значения
X = 3.051522 
Y = 1.582542
Z = 15.62388 
SEQ_COUNT = 400
D_XYZ = 0.04

# Файлы для очистки
CLEAN_FILES = attractor_data.txt research_data.txt sequence.txt fractal.txt

# Правило для вывода помощи
.PHONY: help
help:
	@echo "make attractor_plot - построить график аттрактора"
	@echo "make research_plot - построить график исследования бинарной последовательности"
	@echo "make fractal_plot" - построить тепловую карту
	@echo "make plot1" - построить рисунок 1
	@echo "make plot2" - построить рисунок 2
	@echo "make plot4" - построить рисунок 4
	@echo "make plot6" - построить рисунок 6
	@echo "make clean - удалить вспомогательные файлы"
	@echo "Указать начальные условия можно прописав X=<> Y=<> Z=<> SEQ_COUNT=<> D_XYZ=<> после команды"

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

plot1: clean
	$(CC) $(CFLAGS) lorenz.cpp -o $(EXEC_LORENZ)
	./lorenz 8.051522 6.582542 15.62388 1000 0.2
	gnuplot fractal.plot

plot2: clean
	$(CC) $(CFLAGS) lorenz.cpp -o $(EXEC_LORENZ)
	./lorenz $(X) $(Y) $(Z) 400 0.25
	gnuplot fractal.plot

plot4: clean
	$(CC) $(CFLAGS) lorenz.cpp -o $(EXEC_LORENZ)
	./lorenz 58.051522 $(Y) $(Z) 1500 0.05
	gnuplot fractal.plot

plot6: clean
	$(CC) $(CFLAGS) lorenz.cpp -o $(EXEC_LORENZ)
	./lorenz $(X) $(Y) $(Z) 400 0.02
	gnuplot fractal.plot


# Правило для очистки
clean:
	rm -f $(CLEAN_FILES) $(EXEC_LORENZ)


