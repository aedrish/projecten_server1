package example;
import javax.swing.JFrame;

public class RamMonitor extends JFrame {

	public RamMonitor() {
		setTitle("ramMonitor");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setSize(300, 300);
	}
	
	public static void main(String[] args) {
		RamMonitor rm = new RamMonitor();
		rm.setVisible(true);
	}
	
}
