public class InfiniteExample {

    public static void main(String[] args) throws Exception {
        while (true) {
            Thread.sleep(1);
            subMethod();
        }
    }

    private static void subMethod() {
        System.out.println("calling some code, lalala");
    }

}
