import java.io.BufferedReader;
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
 * A simple WebSocketServer implementation. Keeps track of a "chatroom".
 */
public class ChatServer extends WebSocketServer {
	
	public ChatServer( int port ) throws UnknownHostException {
		super( new InetSocketAddress( port ) );
	}

	public ChatServer( InetSocketAddress address ) {
		super( address );
	}

	@Override
	public void onOpen( WebSocket conn, ClientHandshake handshake ) {
//		connected.add(connected.size(), conn);
	}

	@Override
	public void onClose( WebSocket conn, int code, String reason, boolean remote ) {
//		connected.remove(connected.indexOf(conn));
	}

	@Override
	public void onMessage( WebSocket conn, String message ) {
//		this.sendToAll( message );
		conn.send("received: " +  message );
//		System.out.println( conn + ": " + message );
	}

	@Override
	public void onFragment( WebSocket conn, Framedata fragment ) {
//		System.out.println( "received fragment: " + fragment );
	}

	public static void main( String[] args ) throws InterruptedException , IOException {
		WebSocketImpl.DEBUG = true;
		int port = 8887; // 843 flash policy port
		try {
			port = Integer.parseInt( args[ 0 ] );
		} catch ( Exception ex ) {
		}
		ChatServer s = new ChatServer( port );
		s.start();
		System.out.println( "ChatServer started on port: " + s.getPort() );
	
//		System.out.println( "ChatServer started pinging each 5 seconds" );
//		while ( true ) {
//			s.sendToAll("pong");
//			try {
//				Thread.sleep(5000);	
//			} catch(InterruptedException e) {
//			
//			}
//		}
		
		
		
		BufferedReader sysin = new BufferedReader( new InputStreamReader( System.in ) );
		while ( true ) {
			String in = sysin.readLine();
			s.sendToAll( in );
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
	
	
	@Override
	public void onError( WebSocket conn, Exception ex ) {
		ex.printStackTrace();
		if( conn != null ) {
			// some errors like port binding failed may not be assignable to a specific websocket
		}
	}

	/**
	 * Sends <var>text</var> to all currently connected WebSocket clients.
	 * 
	 * @param text
	 *            The String to send across the network.
	 * @throws InterruptedException
	 *             When socket related I/O errors occur.
	 */
	public void sendToAll( String text ) {
		Collection<WebSocket> con = connections();
		synchronized ( con ) {
			for( WebSocket c : con ) {
				c.send( text );
			}
		}
	}
}
