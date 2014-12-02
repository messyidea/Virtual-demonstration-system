package com.qualcomm.QCARSamples.ImageTargets;



import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;

public class howtouse extends Activity implements OnClickListener{
	private ImageView howimage ;
	private Animation myAnimation_Translate;
	private int cas = 0;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.howtouse);
		howimage = (ImageView)findViewById(R.id.howimage);
		howimage.setOnClickListener(this);
		myAnimation_Translate = AnimationUtils.loadAnimation(this,
				R.anim.my_translate_action);
	}
	
	public void changepic(){
		 howimage.startAnimation(myAnimation_Translate);
		 if(cas == 0){
			 howimage.setImageDrawable(getResources()
						.getDrawable(R.drawable.how2));
		 }
		 else if(cas == 1){
			 howimage.setImageDrawable(getResources()
						.getDrawable(R.drawable.how3));
		 }
		 else if(cas == 2){
			 howimage.setImageDrawable(getResources()
						.getDrawable(R.drawable.how1));
		 }
		 cas = cas + 1;
		 if(cas == 3) cas = 0;
	}
	
	public void onClick(View v)
    {
        switch (v.getId())
        {
        case R.id.howimage:
            changepic();
            break;
        	
        }
    }
	
}
