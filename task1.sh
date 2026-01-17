FIFO_NAME="system_log.fifo"

if [ ! -p "$FIFO_NAME" ]; then
    mkfifo "$FIFO_NAME"
fi

echo "[INFO] Uruchomiono serwer logów. Potok: $FIFO_NAME"

cleanup() {
    trap - SIGINT EXIT 
    echo -e "\n[INFO] Zamykanie serwera. Sprzątanie..."
    rm -f "$FIFO_NAME"
    if [ -n "$READER_PID" ]; then
        kill "$READER_PID" 2>/dev/null
    fi
    exit 0
}
trap cleanup SIGINT EXIT
(
    while true; do
        if read line < "$FIFO_NAME"; then
            echo "[LOG] : $line"
        fi
    done
) &
READER_PID=$!
echo "[INFO] Proces czytający uruchomiony w tle (PID: $READER_PID)."

echo "[LOG] Wysyłam komunikat 1..."
echo "Błąd krytyczny w module X" > "$FIFO_NAME"
sleep 1

echo "[LOG] Wysyłam komunikat 2..."
echo "Użytkownik zalogowany" > "$FIFO_NAME"
sleep 1

echo "[LOG ] Wysyłam komunikat 3..."
echo "Zadanie zakończone sukcesem" > "$FIFO_NAME"
sleep 1

echo "[INFO] Koniec symulacji. Zakończ skrypt (Ctrl+C) lub poczekaj na auto-exit."
