package spw4.connectfour;

import org.junit.jupiter.api.*;
import org.junit.jupiter.api.extension.*;
import org.junit.jupiter.params.*;
import org.junit.jupiter.params.provider.*;
import org.mockito.junit.jupiter.*;

import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

@ExtendWith(MockitoExtension.class)
class ConnectFourImplTests {

    @Nested
    @DisplayName("Constructor ...")
    class ConstructorTests {
        private static Stream<Arguments> playerProvider() {
            return ConnectFourImplTests.playerProvider();
        }

        @DisplayName("Constructor sets the Player from the parameter")
        @ParameterizedTest(name = "Initial Player: {0}")
        @MethodSource("playerProvider")
        public void constrSetsCurrentPlayerCorrectly(Player initPlayer) {
            ConnectFourImpl game = new ConnectFourImpl(initPlayer);

            assertEquals(initPlayer, game.getPlayerOnTurn());
        }

        @DisplayName("Constructor with none as Player throws InvalidPlayerException")
        @Test
        public void constrSetsCurrentPlayerCorrectly() {
            assertThrows(InvalidPlayerException.class, () -> new ConnectFourImpl(Player.none));
        }
    }

    @Nested
    @DisplayName("Drop ...")
    class DropTests {
        private static Stream<Arguments> playerProvider() {
            return ConnectFourImplTests.playerProvider();
        }

        private static Stream<Arguments> playerTurnProvider() {
            return Stream.of(
                    Arguments.of(Player.red, 0),
                    Arguments.of(Player.red, 1),
                    Arguments.of(Player.red, 2),
                    Arguments.of(Player.red, 3),
                    Arguments.of(Player.yellow, 4),
                    Arguments.of(Player.yellow, 5),
                    Arguments.of(Player.yellow, 6)
            );
        }

        @DisplayName("... swaps players for next turn")
        @ParameterizedTest(name = "Initial Player: {0}")
        @MethodSource("playerProvider")
        public void dropPlayerChangesAfterDrop(Player initPlayer) {
            ConnectFourImpl game = new ConnectFourImpl(initPlayer);

            game.drop(1);
            assertEquals(getOpposingPlayer(initPlayer), game.getPlayerOnTurn());
        }

        @DisplayName("... with Player.none as parameter throws InvalidPlayerException")
        @Test
        public void dropWithPlayerNoneThrowsInvalidPlayerException() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);
            game.reset(Player.none);

            assertThrows(InvalidPlayerException.class, () -> game.drop(1));
        }

        @DisplayName("... places the players piece in the correct location")
        @ParameterizedTest(name = "Player {0} places token at col {1}")
        @MethodSource("playerTurnProvider")
        public void dropPlacesPlayersTokenInTheCorrectLocation(Player player, int col) {
            ConnectFourImpl game = new ConnectFourImpl(player);

            game.drop(col);

            assertEquals(player, game.getPlayerAt(0, col));
        }

        @DisplayName("... on invalid column throws IndexOutOfBoundsException")
        @ParameterizedTest(name = "Dropped into column with index {0}")
        @ValueSource(ints = {-1, -10, ConnectFourImpl.BOARD_COLS, ConnectFourImpl.BOARD_COLS + 3})
        public void dropOnInvalidColThrowsIndexOutOfBoundsException(int col) {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            assertThrows(IndexOutOfBoundsException.class, () -> game.drop(col));
        }

        @DisplayName("... to fill entire row works correctly")
        @Test
        public void dropToFillEntireRowWorksCorrectly() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            fillEntireCol(game, 0);

            Player player = Player.red;

            for (int row = 0; row < ConnectFourImpl.BOARD_ROWS; row++) {
                assertEquals(player, game.getPlayerAt(row, 0));
                player = getOpposingPlayer(player);
            }
        }

        @DisplayName("... on full column throws IndexOutOfRangeException")
        @Test
        public void dropFillingAboveRowThrowsIndexOutOfRangeException() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);
            int col = 6;

            fillEntireCol(game, col);

            assertThrows(IndexOutOfBoundsException.class, () -> game.drop(col));
        }
    }

    @Nested
    @DisplayName("reset ...")
    class ResetTests {
        private static Stream<Arguments> playerProvider() {
            return ConnectFourImplTests.playerProvider();
        }

        @DisplayName("... sets current player according to parameter")
        @ParameterizedTest(name = "CurrentPlayer after reset: {0}")
        @MethodSource("playerProvider")
        public void resetChangesCurrentPlayerToParameterPlayer(Player afterResetPlayer) {
            ConnectFourImpl game = new ConnectFourImpl(afterResetPlayer);

            game.reset(afterResetPlayer);
            assertEquals(afterResetPlayer, game.getPlayerOnTurn());
        }

        @DisplayName("... empties the entire board")
        @Test
        public void resetChangesCurrentPlayerToParameterPlayer() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            ConnectFourImpl gameOriginal = new ConnectFourImpl(Player.yellow);

            fillEntireBoard(game);

            game.reset(Player.yellow);

            assertEquals(gameOriginal.toString(), game.toString());
        }
    }

    @Nested
    @DisplayName("getPlayerAt ...")
    class GetPlayerAtTests {
        private static Stream<Arguments> invalidPositionProvider() {
            return ConnectFourImplTests.invalidPositionProvider();
        }

        @DisplayName("... with invalid position throws IndexOutOfBoundsException")
        @ParameterizedTest(name = "Dropped into column with row {0} and col {1}")
        @MethodSource("invalidPositionProvider")
        public void getPlayerAtWithInvalidPositionThrowsIndexOutOfBoundsException(int row, int col) {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            assertThrows(IndexOutOfBoundsException.class, () -> game.getPlayerAt(row, col));
        }

        @DisplayName("... on empty position returns Player.none")
        @Test
        public void getPlayerAtOnEmptyPositionReturnsPlayerNone() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            for (int rows = 0; rows < ConnectFourImpl.BOARD_ROWS; rows++) {
                for (int cols = 0; cols < ConnectFourImpl.BOARD_COLS; cols++) {
                    assertEquals(Player.none, game.getPlayerAt(rows, cols));
                }
            }
        }
    }

    @Nested
    @DisplayName("toString ...")
    class ToStringTests {
        private static Stream<Arguments> playerProvider() {
            return ConnectFourImplTests.playerProvider();
        }

        @DisplayName("... prints filled board correctly")
        @Test
        public void toStringPrintsFilledBoardCorrectly() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            fillEntireBoard(game);

            StringBuilder expectedBuilder = new StringBuilder();
            expectedBuilder.append("Player: %s\n".formatted(game.getPlayerOnTurn().toString().toUpperCase()));

            for (int row = ConnectFourImpl.BOARD_ROWS - 1; row >= 0; row--) {
                expectedBuilder.append("|");
                for (int col = 0; col < ConnectFourImpl.BOARD_COLS; col++) {
                    expectedBuilder.append(" ");

                    switch (game.getPlayerAt(row, col)) {
                        case red -> expectedBuilder.append("R");
                        case yellow -> expectedBuilder.append("Y");
                    }

                    expectedBuilder.append(" ");
                }
                expectedBuilder.append("|\n");
            }

            assertEquals(expectedBuilder.toString(), game.toString());
        }

        @DisplayName("... initial game board prints as empty board and current player correctly")
        @ParameterizedTest(name = "Player in output: {0}")
        @MethodSource("playerProvider")
        public void toStringOnInitialGameBoardPrintsAsEmptyBoard(Player outputPlayer) {
            ConnectFourImpl game = new ConnectFourImpl(outputPlayer);

            String expectedString = "Player: %s\n".formatted(outputPlayer.toString().toUpperCase()) +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n";

            assertEquals(expectedString, game.toString());
        }

        @DisplayName("... prints board with bottom row filled correctly")
        @Test
        public void toStringPrintsBoardWithBottomLayerCorrectly() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            for (int col = 0; col < ConnectFourImpl.BOARD_COLS; col++) {
                game.drop(col);
            }

            String expectedString = "Player: YELLOW\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| .  .  .  .  .  .  . |\n" +
                    "| R  Y  R  Y  R  Y  R |\n";
            assertEquals(expectedString, game.toString());
        }
    }

    @Nested
    @DisplayName("Win Condition...")
    class WinConditions {
        @DisplayName("... Full Board with no win condition met is game over and winner is Player.none")
        @Test
        public void fullBoardWithNoWinConditionIsGameOverWithoutWinner() {
            ConnectFourImpl game = createGameWithFullBoardWithoutWinner();

            assertAll(
                    () -> assertTrue(game.isGameOver()),
                    () -> assertEquals(Player.none, game.getWinner())
            );
        }

        @DisplayName("...horizontal alignment 4 row and red won")
        @Test
        public void boardWithHorizontalWinCondition() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);
            int winCount = 4;

            for (int col = 0; col < winCount - 1; col++) {
                game.drop(col);
                game.drop(col);
            }
            game.drop(winCount - 1);

            assertAll(
                    () -> assertTrue(game.isGameOver()),
                    () -> assertEquals(Player.red, game.getWinner())
            );
        }

        @DisplayName("...vertical alignment 4 row and yellow won")
        @Test
        public void boardWithVerticalWinCondition() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);
            int winCount = 4;

            game.drop(6); // 1x drop to also check for yellow win condition
            for (int row = 0; row < winCount * 2 - 1; row++) {
                game.drop(row % 2);
            }

            assertAll(
                    () -> assertTrue(game.isGameOver()),
                    () -> assertEquals(Player.yellow, game.getWinner())
            );
        }

        @DisplayName("...diagonal forwards 4 row and red won")
        @ParameterizedTest(name="diagonal starting from 0 0 to 3 3 with a col offset of {0}")
        @ValueSource(ints = {0, 1, 2})
        public void boardWithForwardDiagonalWinCondition(int colOffset) {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);
            fillGameWithForwardDiagonalConditionWinWithColOffset(game, colOffset);

            assertAll(
                    () -> assertTrue(game.isGameOver()),
                    () -> assertEquals(Player.red, game.getWinner())
            );
        }

        @DisplayName("...diagonal forwards 4 row and row offset and yellow won")
        @ParameterizedTest(name="diagonal starting from 0 0 to 3 3 with a col offset of {0}")
        @ValueSource(ints = {0, 1, 2})
        public void boardWithRowOffsetAndForwardDiagonalWinCondition(int colOffset) {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);
            fillEntireRow(game);

            fillGameWithForwardDiagonalConditionWinWithColOffset(game, colOffset);

            assertAll(
                    () -> assertTrue(game.isGameOver()),
                    () -> assertEquals(Player.yellow, game.getWinner())
            );
        }

        @DisplayName("...diagonal backwards 4 row and red won")
        @ParameterizedTest(name="diagonal starting from 3 0 to 0 3 with a col offset of {0}")
        @ValueSource(ints = {0, 1, 2})
        public void boardWithBackwardsDiagonalWinCondition(int colOffset) {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            fillGameWithBackwardDiagonalConditionWinWithColOffset(game, colOffset);

            assertAll(
                    () -> assertTrue(game.isGameOver()),
                    () -> assertEquals(Player.red, game.getWinner())
            );
        }

        @DisplayName("... is not met if board is still empty")
        @Test
        public void emptyBoardGameIsNotOver() {
            ConnectFourImpl game = new ConnectFourImpl(Player.red);

            assertFalse(game.isGameOver());
        }
    }

    private static void fillGameWithForwardDiagonalConditionWinWithColOffset(ConnectFourImpl game, int colOffset) {
        game.drop(colOffset);
        game.drop(1 + colOffset);

        game.drop(1 + colOffset);
        game.drop(2 + colOffset);

        game.drop(2 + colOffset);
        game.drop(3 + colOffset);

        game.drop(2 + colOffset);
        game.drop(3 + colOffset);

        game.drop(4 + colOffset);
        game.drop(3 + colOffset);

        game.drop(3 + colOffset);
    }

    private static void fillGameWithBackwardDiagonalConditionWinWithColOffset(ConnectFourImpl game, int colOffset) {
        int baseline = 4 + colOffset;

        game.drop(baseline);
        game.drop(baseline - 1);

        game.drop(baseline - 1);
        game.drop(baseline - 2);

        game.drop(baseline - 2);
        game.drop(baseline - 3);

        game.drop(baseline - 2);
        game.drop(baseline - 3);

        game.drop(baseline - 4);
        game.drop(baseline - 3);

        game.drop(baseline - 3);
    }

    private static ConnectFourImpl createGameWithFullBoardWithoutWinner() {
        ConnectFourImpl game = new ConnectFourImpl(Player.red);

        /** For Visual guidance
         *
         * | Y Y Y R R Y Y |
         * | R R R Y Y R R |
         * | Y Y Y R Y R R |
         * | R R R Y Y R R |
         * | Y Y Y R R Y Y |
         * | R R R Y R Y Y |
         * | Y Y Y R R Y Y |
         */

        fillEntireCol(game, 3);
        game.drop(4);

        fillEntireCol(game, 0);
        fillEntireCol(game, 1);
        fillEntireCol(game, 2);

        game.drop(5);
        game.drop(4);
        game.drop(5);
        game.drop(4);
        game.drop(5);

        game.drop(5);
        game.drop(6);
        game.drop(5);
        game.drop(6);
        game.drop(5);
        game.drop(6);

        game.drop(6);
        game.drop(4);
        game.drop(6);
        game.drop(4);
        game.drop(6);
        game.drop(4);

        return game;
    }

    private static void fillEntireBoard(ConnectFourImpl game) {
        for (int col = 0; col < ConnectFourImpl.BOARD_COLS; col++) {
            fillEntireCol(game, col);
        }
    }

    private static void fillEntireCol(ConnectFourImpl game, int col) {
        for (int row = 0; row < ConnectFourImpl.BOARD_ROWS; row++) {
            game.drop(col);
        }
    }

    private static void fillEntireRow(ConnectFourImpl game) {
        for (int col = 0; col < ConnectFourImpl.BOARD_COLS; col++) {
            game.drop(col);
        }
    }

    private static Player getOpposingPlayer(Player player) {
        return player == Player.yellow ? Player.red : Player.yellow;
    }

    private static Stream<Arguments> playerProvider() {
        return Stream.of(
                Arguments.of(Player.red),
                Arguments.of(Player.yellow)
        );
    }

    private static Stream<Arguments> invalidPositionProvider() {
        return Stream.of(
                Arguments.of(-10, 15),
                Arguments.of(0, -3),
                Arguments.of(ConnectFourImpl.BOARD_ROWS, 0),
                Arguments.of(0, ConnectFourImpl.BOARD_COLS),
                Arguments.of(ConnectFourImpl.BOARD_ROWS, ConnectFourImpl.BOARD_COLS),
                Arguments.of(ConnectFourImpl.BOARD_ROWS + 1, ConnectFourImpl.BOARD_COLS + 1),
                Arguments.of(-5, -10)
        );
    }
}
