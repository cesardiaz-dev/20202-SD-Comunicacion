package cliente;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

import compartido.Calculadora;

public class Cliente {
    public static void main(String args[]) {
        double num1;
        double num2;

        try (Scanner scan = new Scanner(System.in)) {
            String name = "Calculadora";
            Registry registry = LocateRegistry.getRegistry(args[0]);
            Calculadora comp = (Calculadora) registry.lookup(name);

            System.out.printf("Entre el numero 1: ");
            num1 = scan.nextDouble();
            System.out.printf("Entre el numero 2: ");
            num2 = scan.nextDouble();

            double resp = comp.suma(num1, num2);

            System.out.printf("El resultado es: %.2f\n", resp);
        } catch (Exception e) {
            System.err.println("Excepcion en Cliente:");
            e.printStackTrace();
        }
    }
}
