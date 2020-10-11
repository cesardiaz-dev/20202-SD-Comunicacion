import distribuidos.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CORBA.*;
import java.io.*;
import java.util.Scanner;

public class Cliente {

    public static void main(String args[]) {

        try {
            // Iniciar el ORB
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);

            // Obtener la raiz del contexto de nombres
            org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
            NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

            // Resuelve el objeto usando el nombre registradp
            Calculadora calc = CalculadoraHelper.narrow(ncRef.resolve_str("Calculadora"));

            try (Scanner scan = new Scanner(System.in)) {
                System.out.printf("Entre el numero 1: ");
                double num1 = scan.nextDouble();
                System.out.printf("Entre el numero 2: ");
                double num2 = scan.nextDouble();

                // Usar la calculadora
                double resp = calc.add(num1, num2);

                System.out.printf("El resultado es: %.2f\n", resp);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
