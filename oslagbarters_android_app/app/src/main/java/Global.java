import android.app.Application;

public class Global extends Application {

    private int mGlobalVarValue;

    public int getGlobalVarValue() {
        return mGlobalVarValue;
    }

    public void setGlobalVarValue(int i) {
        mGlobalVarValue = i;
    }
}