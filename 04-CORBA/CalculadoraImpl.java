public class CalculadoraImpl extends distribuidos.CalculadoraPOA {
    private double memory_;

    public CalculadoraImpl(){ 
        memory_ = 0;
    }

    public double add(double x,double y) {
        return x + y;
    }

    public double substract(double x,double y) {
        return x - y;
    }

    public double multiply(double x,double y){
        return x * y;
    }

    public double divide(double x,double y){
        double result = 0;
        try {
            result = x / y;
        }catch(Exception e){
        }
        return result;
    }

    public void storeMemory(double x){
        memory_ = x;
    }

    public double readMemory(){
        return memory_;
    }
}