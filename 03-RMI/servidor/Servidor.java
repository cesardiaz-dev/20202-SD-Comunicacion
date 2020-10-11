package servidor;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import compartido.Calculadora;

public class Servidor {

    public static void main(String[] args) {
        try {
            String name = "Calculadora";
            Calculadora calc = new CalculadoraImpl();
            Calculadora stub = (Calculadora) UnicastRemoteObject.exportObject(calc, 0);
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind(name, stub);
            System.out.println("Servidor corriendo...");
        } catch (Exception e) {
            System.err.println("Excepcion en Servidor:");
            e.printStackTrace();
        }
    }
}
