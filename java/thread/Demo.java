package lgm;

public class Demo implements Runnable {

	Demo() {
		try {
			new Thread(this).start();
			Thread.sleep(1000);
			new Thread(this).start();
			
			while (true) {
				System.out.println("\t\tmain thread");
				Thread.sleep(100);
			}

		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void run() {
		
		while (true) {
			System.out.println("second thread " );
			try {
				Thread.sleep(200);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("my first multi-thread application...V0.0");
		new Demo();
		System.out.println("bye !");
	}

}
