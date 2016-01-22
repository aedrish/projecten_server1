import java.awt.GridLayout;
import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.channels.ShutdownChannelGroupException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Timer;
import java.util.logging.Logger;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.java_websocket.WebSocket;
import org.java_websocket.WebSocketImpl;
import org.java_websocket.framing.Framedata;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

import java.util.Timer;
import java.util.TimerTask;
/**
 * A simple WebSocketServer implementation. Keeps track of a "chat room".
 */
public class ServerPerformanceTest extends WebSocketServer {
	
	public ServerPerformanceTest( int port ) throws UnknownHostException {
		super( new InetSocketAddress( port ) );
	}

	public ServerPerformanceTest( InetSocketAddress address ) {
		super( address, 4 );
	}

	@Override
	public void onOpen(WebSocket conn, ClientHandshake handshake) {
//		System.out.println("Client hassss connected, " + connections().size() + " clients connected");
		
	}

	@Override
	public void onClose(WebSocket conn, int code, String reason, boolean remote) {
//		System.out.println("Client hassss disconnected, " + connections().size() + " clients are connected");
		
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
	
//	public static class RamMonitor extends JFrame {
//
//		private JSlider rms;
//		private JLabel rmsl = new JLabel();
//		
//		public RamMonitor() {
//			setTitle("ramMonitor");
//			setDefaultCloseOperation(EXIT_ON_CLOSE);
//			setSize(300, 300);
//			rms = new JSlider(0, 16000000);
//			rms.addChangeListener(new ChangeListener() {
//				
//				@Override
//				public void stateChanged(ChangeEvent e) {
//					rmsl.setText("ram usage: " + rms.getValue());
//				}
//			});
//			setLayout(new GridLayout(2, 1, 10, 10));
//			add(rmsl);
//			add(rms);
//			setVisible(true);
//			Thread thread = new Thread(new Runnable() {
//				
//				@Override
//				public void run() {
//					while(true) {
//						runThread();
//					}
//				}
//			});
//			thread.start();
//			System.out.println("test");
//		}
//		
//		public void runThread() {
//			Runtime rt = Runtime.getRuntime();
//			long usedMB = (rt.totalMemory() - rt.freeMemory()) / 1024;
////			System.out.println("used: + " + usedMB);
//			rms.setValue((int) usedMB);
//		}
//	}
	
	public static void showRamUsage() {
		Runtime runtime = Runtime.getRuntime();
		System.out.println("Free Memmory: " + (runtime.freeMemory() /1024 / 1024) + "MB");
		System.out.println("Total Memmory: " + (runtime.totalMemory() / 1024 / 1024) + "MB");
		System.out.println("used Memmory: " + ((runtime.totalMemory() - runtime.freeMemory()) /1024 /1024) + "MB");
		
	}
	
	public static void main( String[] args ) throws InterruptedException , IOException {
		WebSocketImpl.DEBUG = false;


//		RamMonitor rm = new RamMonitor();
//		rm.setVisible(true);
//		rm.setVisible();
		Timer timer = new Timer(true);
		timer.scheduleAtFixedRate(new TimerTask() {
			@Override
			public void run() {
				showRamUsage();
			}
		}, 0, 250);
		
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
