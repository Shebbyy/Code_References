package spw4.connectfour;

public class ConnectFourImpl implements ConnectFour {
    public static final int BOARD_ROWS = 6;
    public static final int BOARD_COLS = 7;

    private Player currentPlayer;

    private final Player[][] board;

    public ConnectFourImpl(Player playerOnTurn) throws InvalidPlayerException {
        if (playerOnTurn == Player.none) {
            throw new InvalidPlayerException();
        }

        currentPlayer = playerOnTurn;

        board = new Player[BOARD_ROWS][BOARD_COLS];

        initializeBoard();
    }

    private void initializeBoard() {
        for (int row = 0; row < BOARD_ROWS; row++) {
            for (int col = 0; col < BOARD_COLS; col++) {
                board[row][col] = Player.none;
            }
        }
    }

    public Player getPlayerAt(int row, int col) {
        if (   !isBoundValid(col, BOARD_COLS)
                || !isBoundValid(row, BOARD_ROWS)) {
            throw new IndexOutOfBoundsException();
        }

        return board[row][col];
    }

    public Player getPlayerOnTurn() {
        return currentPlayer;
    }

    public boolean isGameOver() {
        return getWinner() != Player.none
                || isBoardFull();
    }

    private boolean isBoardFull() {
        for (int col = 0; col < BOARD_COLS; col++) {
            if (board[BOARD_ROWS - 1][col] == Player.none) {
                return false;
            }
        }

        return true;
    }

    public Player getWinner() {
        if (   isHorizontalWinConditionMet()
                || isVerticalWinConditionMet()
                || isCrossWinConditionMet(0, 1)
                || isCrossWinConditionMet( BOARD_COLS - 1, -1)) {
            return getPlayerOnTurn() == Player.red ? Player.yellow : Player.red;
        }

        return Player.none;
    }

    private boolean isCrossWinConditionMet(int colStart, int colAdjustment) {
        for (int colModifier = colStart; isBoundValid(colModifier, BOARD_COLS); colModifier += colAdjustment) {
            for (int rowModifier = 0; isBoundValid(rowModifier, BOARD_COLS); rowModifier++) {
                int i = 0;
                int count = 0;
                Player lastPlayer = Player.none;

                while (   isBoundValid(rowModifier + i, BOARD_ROWS)
                        && isBoundValid(colModifier + i * colAdjustment, BOARD_COLS)) {
                    Player curPlayer = getPlayerAt(i + rowModifier, i * colAdjustment + colModifier);
                    if (   curPlayer  != Player.none
                            && curPlayer == lastPlayer) {
                        count++;
                    } else {
                        count = 1;
                        lastPlayer = curPlayer;
                    }

                    if (count == 4) {
                        return true;
                    }
                    i++;
                }
            }
        }


        return false;
    }

    private boolean isVerticalWinConditionMet() {
        Player lastPlayer = Player.none;
        for (int row = 0; row < BOARD_ROWS; row++) {
            int count = 0;
            for (int col = 0; col < BOARD_COLS; col++) {
                Player curPlayer = getPlayerAt(row, col);
                if (   lastPlayer == curPlayer
                        && curPlayer != Player.none) {
                    count++;
                } else {
                    count = 1;
                    lastPlayer = curPlayer;
                }

                if (count == 4) {
                    return true;
                }
            }
        }

        return false;
    }

    private boolean isHorizontalWinConditionMet() {
        Player lastPlayer = Player.none;
        for (int col = 0; col < BOARD_COLS; col++) {
            int count = 0;
            for (int row = 0; row < BOARD_ROWS; row++) {
                Player curPlayer = getPlayerAt(row, col);
                if (   lastPlayer == curPlayer
                        && curPlayer != Player.none) {
                    count++;
                } else {
                    count = 1;
                    lastPlayer = curPlayer;
                }

                if (count == 4) {
                    return true;
                }
            }
        }

        return false;
    }

    @Override
    public String toString() {
        StringBuilder output = new StringBuilder().append("Player: %s\n".formatted(currentPlayer.toString().toUpperCase()));

        for (int row = BOARD_ROWS - 1; row >= 0; row--) {
            output.append("|");
            for (int col = 0; col < BOARD_COLS; col++) {
                output.append(" ");

                switch (getPlayerAt(row, col)) {
                    case red -> output.append("R");
                    case yellow -> output.append("Y");
                    default -> output.append(".");
                }

                output.append(" ");
            }
            output.append("|\n");
        }

        return output.toString();
    }

    public void reset(Player playerOnTurn) {
        initializeBoard();
        currentPlayer = playerOnTurn;
    }

    public void drop(int col) {
        if (!isBoundValid(col, BOARD_COLS)) {
            throw new IndexOutOfBoundsException();
        }

        Player lastPlayer = switchPlayer();

        int row = 0;
        while (   isBoundValid(row, BOARD_ROWS)
                && board[row][col] != Player.none) {
            row++;
        }

        if (!isBoundValid(row, BOARD_ROWS)) {
            throw new IndexOutOfBoundsException();
        }

        board[row][col] = lastPlayer;
    }

    private Player switchPlayer() {
        Player lastPlayer = currentPlayer;
        switch (currentPlayer) {
            case red -> currentPlayer = Player.yellow;
            case yellow -> currentPlayer = Player.red;
            default -> throw new InvalidPlayerException();
        }

        return lastPlayer;
    }

    private boolean isBoundValid(int value, int upperBound) {
        return value >= 0
                && value < upperBound;
    }
}
