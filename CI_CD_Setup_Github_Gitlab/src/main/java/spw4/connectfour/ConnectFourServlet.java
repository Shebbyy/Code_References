package spw4.connectfour;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.UUID;

@WebServlet("/ConnectFour")
public class ConnectFourServlet extends HttpServlet {
    private HashMap<UUID, ConnectFour> games;

    @Override
    public void init() throws ServletException {
        super.init();
        games = new HashMap<>();
    }

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("text/html");
        PrintWriter writer = response.getWriter();

        try {
            String action = request.getParameter("action");
            if (action == null) action = "";

            if ("new".equals(action)) {
                UUID id = UUID.randomUUID();
                ConnectFour newGame = new ConnectFourImpl(Player.red);
                games.put(id, newGame);
                writer.println(game2Html(id, newGame));
                return;
            }

            String idString = request.getParameter("id");
            if (idString != null) {
                UUID id = UUID.fromString(idString);
                ConnectFour game = games.get(id);

                if (!game.isGameOver()) {
                    switch (action) {
                        case "c1":
                            game.drop(0);
                            break;
                        case "c2":
                            game.drop(1);
                            break;
                        case "c3":
                            game.drop(2);
                            break;
                        case "c4":
                            game.drop(3);
                            break;
                        case "c5":
                            game.drop(4);
                            break;
                        case "c6":
                            game.drop(5);
                            break;
                        case "c7":
                            game.drop(6);
                            break;
                    }
                }

                writer.println(game2Html(id, games.get(id)));
                return;
            }

            writer.println("invalid request");
        }
        catch (Throwable t) {
            writer.println(t);
        }
    }

    private String game2Html(UUID id, ConnectFour game) {
        StringBuffer sb = new StringBuffer();

        sb.append("<!DOCTYPE html>\n");
        sb.append("<html lang=\"en\">\n");
        sb.append("<head>\n");
        sb.append("    <meta charset=\"UTF-8\">\n");
        sb.append("    <title>Connect Four</title>\n");
        sb.append("    <link href=\"css/bootstrap.min.css\" rel=\"stylesheet\"/>\n");
        sb.append("    <link href=\"icons/bootstrap-icons.css\" rel=\"stylesheet\"/>\n");
        sb.append("    <script src=\"js/bootstrap.min.js\"></script>\n");
        sb.append("</head>\n");
        sb.append("<body>\n");
        sb.append("    <h1 class=\"m-5 text-center\">Play Connect Four:</h1>\n");
        sb.append("    <div class=\"row\">\n");
        sb.append("        <div class=\"col\"></div>\n");
        sb.append("        <div class=\"col\">\n");
        sb.append("            <div class=\"row text-center pb-4\">\n");
        sb.append("                <div class=\"col\"><h5>Player: " + game.getPlayerOnTurn().toString().toUpperCase() + "</h5></div>\n");
        sb.append("            </div>\n");
        sb.append("            <div class=\"row\">\n");
        sb.append("                <div class=\"col p-0\">\n");
        sb.append("                    <div class=\"container p-0 text-center bg-secondary\">\n");
        sb.append("                        <div class=\"row m-0 bg-white\">\n");
        sb.append("                            <div class=\"col p-1 ml-1\"><a role=\"button\" class=\"btn btn-secondary p-1\" href=\"ConnectFour?id=" + id + "&action=c1\"><i class=\"bi-arrow-down\"></i></a></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1\"><a role=\"button\" class=\"btn btn-secondary p-1\" href=\"ConnectFour?id=" + id + "&action=c2\"><i class=\"bi-arrow-down\"></i></a></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1\"><a role=\"button\" class=\"btn btn-secondary p-1\" href=\"ConnectFour?id=" + id + "&action=c3\"><i class=\"bi-arrow-down\"></i></a></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1\"><a role=\"button\" class=\"btn btn-secondary p-1\" href=\"ConnectFour?id=" + id + "&action=c4\"><i class=\"bi-arrow-down\"></i></a></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1\"><a role=\"button\" class=\"btn btn-secondary p-1\" href=\"ConnectFour?id=" + id + "&action=c5\"><i class=\"bi-arrow-down\"></i></a></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1\"><a role=\"button\" class=\"btn btn-secondary p-1\" href=\"ConnectFour?id=" + id + "&action=c6\"><i class=\"bi-arrow-down\"></i></a></div>\n");
        sb.append("                            <div class=\"col p-1 mx-1\"><a role=\"button\" class=\"btn btn-secondary p-1\" href=\"ConnectFour?id=" + id + "&action=c7\"><i class=\"bi-arrow-down\"></i></a></div>\n");
        sb.append("                        </div>\n");
        sb.append("                        <div class=\"row pt-1 m-0\" style=\"height:36px\">\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 0, 0) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 0, 1) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 0, 2) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 0, 3) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 0, 4) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 0, 5) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 mx-1 " + getCellColor(game, 0, 6) + "\"></div>\n");
        sb.append("                        </div>\n");
        sb.append("                        <div class=\"row pt-1 m-0\" style=\"height:36px\">\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 1, 0) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 1, 1) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 1, 2) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 1, 3) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 1, 4) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 1, 5) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 mx-1 " + getCellColor(game, 1, 6) + "\"></div>\n");
        sb.append("                        </div>\n");
        sb.append("                        <div class=\"row pt-1 m-0\" style=\"height:36px\">\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 2, 0) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 2, 1) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 2, 2) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 2, 3) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 2, 4) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 2, 5) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 mx-1 " + getCellColor(game, 2, 6) + "\"></div>\n");
        sb.append("                        </div>\n");
        sb.append("                        <div class=\"row pt-1 m-0\" style=\"height:36px\">\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 3, 0) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 3, 1) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 3, 2) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 3, 3) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 3, 4) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 3, 5) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 mx-1 " + getCellColor(game, 3, 6) + "\"></div>\n");
        sb.append("                        </div>\n");
        sb.append("                        <div class=\"row pt-1 m-0\" style=\"height:36px\">\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 4, 0) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 4, 1) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 4, 2) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 4, 3) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 4, 4) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 4, 5) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 mx-1 " + getCellColor(game, 4, 6) + "\"></div>\n");
        sb.append("                        </div>\n");
        sb.append("                        <div class=\"row py-1 m-0\" style=\"height:40px\">\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 5, 0) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 5, 1) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 5, 2) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 5, 3) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 5, 4) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 ml-1 " + getCellColor(game, 5, 5) + "\"></div>\n");
        sb.append("                            <div class=\"col p-1 mx-1 " + getCellColor(game, 5, 6) + "\"></div>\n");
        sb.append("                        </div>\n");
        sb.append("                    </div>\n");
        sb.append("                </div>\n");
        sb.append("            </div>\n");
        if (game.isGameOver() && game.getWinner() == Player.yellow) {
            sb.append("                <div class=\"row text-center pt-4\">\n");
            sb.append("                    <div class=\"col\"><h1><span class=\"badge bad badge-warning w-75\">YELLOW WINS</span></h1></div>\n");
            sb.append("                </div>\n");
        } else if (game.isGameOver() && game.getWinner() == Player.red) {
            sb.append("                <div class=\"row text-center pt-4\">\n");
            sb.append("                    <div class=\"col\"><h1><span class=\"badge badge-danger w-75\">RED WINS</span></h1></div>\n");
            sb.append("                </div>\n");
        } else if (game.isGameOver() && game.getWinner() == Player.none) {
            sb.append("                <div class=\"row text-center pt-4\">\n");
            sb.append("                    <div class=\"col\"><h1><span class=\"badge badge-secondary w-75\">DRAW</span></h1></div>\n");
            sb.append("                </div>\n");
        }
        sb.append("            <div class=\"row text-center pt-4\">\n");
        sb.append("                <div class=\"col\"><a role=\"button\" class=\"btn btn-danger w-75\" href=\"ConnectFour?action=new\">New Game</a></div>\n");
        sb.append("                <div class=\"col\"><a role=\"button\" class=\"btn btn-info w-75\" href=\"ConnectFour?id=" + id + "\">Refresh</a></div>\n");
        sb.append("            </div>\n");
        sb.append("        </div>\n");
        sb.append("        <div class=\"col\"></div>\n");
        sb.append("    </div>\n");
        sb.append("</body>\n");
        sb.append("</html>\n");

        return sb.toString();
    }

    private String getCellColor(ConnectFour game, int row, int col) {
        return switch (game.getPlayerAt(row, col)) {
            case red -> "bg-danger";
            case yellow -> "bg-warning";
            default -> "bg-light";
        };
    }
}
