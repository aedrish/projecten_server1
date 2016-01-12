import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collection;

import org.java_websocket.WebSocket;
import org.java_websocket.WebSocketImpl;
import org.java_websocket.framing.Framedata;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;
/**
 * A simple WebSocketServer implementation. Keeps track of a "chat room".
 */
public class ServerPerformanceTest extends WebSocketServer {
	
	public ServerPerformanceTest( int port ) throws UnknownHostException {
		super( new InetSocketAddress( port ) );
	}

	public ServerPerformanceTest( InetSocketAddress address ) {
		super( address );
	}

	@Override
	public void onOpen(WebSocket conn, ClientHandshake handshake) {
		System.out.println("Client has connected, " + connections().size() + " clients connected");
		
	}

	@Override
	public void onClose(WebSocket conn, int code, String reason, boolean remote) {
		System.out.println("Client has disconnected, " + connections().size() + " clients are connected");
		
	}

	@Override
	public void onMessage(WebSocket conn, String message) {
		conn.send(message);
	}

	@Override
	public void onError(WebSocket conn, Exception ex) {
		try {
			conn.send("Error occured");
		} catch(Exception e) {
			System.out.println("Error occured and coulnd't send message to client");
		}
		
	}
	
	public static void main( String[] args ) throws InterruptedException , IOException {
		WebSocketImpl.DEBUG = true;
		int port = 8887;
		try {
			port = Integer.parseInt( args[ 0 ] );
		} catch ( Exception ex ) {
		}
		ServerPerformanceTest s = new ServerPerformanceTest( port );
		s.start();
		System.out.println( "ChatServer started on port: " + s.getPort() );
		
		BufferedReader sysin = new BufferedReader( new InputStreamReader( System.in ) );
		while ( true ) {
			String in = sysin.readLine();
			if( in.equals( "exit" ) ) {
				s.stop();
				break;
			} else if( in.equals( "restart" ) ) {
				s.stop();
				s.start();
				break;
			}
		}
		
	}
	
}
