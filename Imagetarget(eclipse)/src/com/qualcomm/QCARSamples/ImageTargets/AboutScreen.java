/*==============================================================================
Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
All Rights Reserved.

@file
    AboutScreen.java

@brief
    About Screen Activity for the ImageTargets sample application

==============================================================================*/

package com.qualcomm.QCARSamples.ImageTargets;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Html;
import android.text.method.LinkMovementMethod;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class AboutScreen extends Activity implements OnClickListener
{
    //private TextView mAboutText;
    //private Button mStartButton;
	private ImageView startc;
	private ImageView aboutit;
	private ImageView howtouse;


    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.startmyproject);
        startc = (ImageView)findViewById(R.id.startc);
        startc.setOnClickListener(this);
        aboutit = (ImageView)findViewById(R.id.aboutit);
        aboutit.setOnClickListener(this);
        howtouse = (ImageView)findViewById(R.id.howtouse);
        howtouse.setOnClickListener(this);

       /* mAboutText = (TextView) findViewById(R.id.about_text);
        mAboutText.setText(Html.fromHtml(getString(R.string.about_text)));
        mAboutText.setMovementMethod(LinkMovementMethod.getInstance());

        // Setups the link color
        mAboutText.setLinkTextColor(getResources().getColor(
                R.color.holo_light_blue));

        mStartButton = (Button) findViewById(R.id.button_start);
        mStartButton.setOnClickListener(this);*/
    }


    /** Starts the ImageTargets main activity */
    private void startARActivity()
    {
        Intent i = new Intent(this, ImageTargets.class);
        startActivity(i);
    }
    
    private void startARActivityabout()
    {
        Intent i = new Intent(this, about.class);
        startActivity(i);
    }
    
    private void startARActivityhow()
    {
        Intent i = new Intent(this, howtouse.class);
        startActivity(i);
    }
    


    public void onClick(View v)
    {
        switch (v.getId())
        {
        case R.id.startc:
            startARActivity();
            break;
        case R.id.aboutit:
        	startARActivityabout();
        	break;
        case R.id.howtouse:
        	startARActivityhow();
        	break;
        	
        }
    }
}
