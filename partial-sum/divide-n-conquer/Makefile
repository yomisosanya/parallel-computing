CC = mpicc
TARGET = DCParallelSum
FLAGS = -o $(TARGET) -lm
SOURCE = DCParallelSum.c
$(TARGET):
	$(CC) $(FLAGS) $(SOURCE)
	@echo "$(TARGET) created"
clean:
	rm $(TARGET)
	@echo "$(TARGET) removed"
