echo "Сборка проекта..."
make || {
    echo "Ошибка компиляции.";
    exit 1;
}

echo "Запуск сервера на http://localhost:8081"
./server

echo "Сервер остановлен."