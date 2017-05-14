package com.divol13.magic2048;

import org.oxygine.lib.OxygineActivity;
//import org.oxygine.billing.BillingAmazon;
//import org.oxygine.billing.BillingGoogle;
import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.os.RemoteException;

import com.google.android.gms.analytics.GoogleAnalytics;
import com.google.android.gms.analytics.Tracker;
import com.google.android.gms.analytics.HitBuilders;

import android.os.Bundle;
import com.flurry.android.FlurryAgent;
import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.EnumSet;
import android.util.Log;


import android.content.pm.PackageManager.NameNotFoundException;

//import org.oxygine.lib.R;
import com.divol13.magic2048.R;

import android.content.Intent;
import android.content.IntentSender.SendIntentException;


import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.plus.Plus;

import com.google.android.gms.games.achievement.Achievements.UpdateAchievementResult;

import com.google.android.gms.games.leaderboard.Leaderboards;
import com.google.android.gms.common.api.ResultCallback;
import android.support.v4.app.FragmentActivity;

import com.google.android.vending.licensing.*;

import android.widget.Toast;

import android.app.Notification;
import android.app.Notification.Builder;
import org.libsdl.app.NotificationPublisher;
import org.libsdl.app.SampleAlarmReceiver;
import org.libsdl.app.SampleDownloaderService;

import android.net.Uri;


import com.chartboost.sdk.CBLocation;
import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDelegate;
import com.chartboost.sdk.InPlay.CBInPlay;
import com.chartboost.sdk.Libraries.CBLogging.Level;
import com.chartboost.sdk.Model.CBError.CBClickError;
import com.chartboost.sdk.Model.CBError.CBImpressionError;
import com.chartboost.sdk.Tracking.CBAnalytics;

import android.app.AlarmManager;
import android.os.SystemClock;

import android.app.PendingIntent;
import android.content.Context;


import com.android.vending.expansion.zipfile.ZipResourceFile;
import com.android.vending.expansion.zipfile.ZipResourceFile.ZipEntryRO;
import com.google.android.vending.expansion.downloader.Constants;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;

import android.os.Message;
import android.os.Messenger;


import com.amazon.device.iap.PurchasingService;
import com.amazon.device.iap.model.RequestId;

import com.amazon.ags.api.AGResponseCallback;
import com.amazon.ags.api.AGResponseHandle;
import com.amazon.ags.api.AmazonGamesCallback;
import com.amazon.ags.api.AmazonGamesFeature;
import com.amazon.ags.api.AmazonGamesStatus;
import com.amazon.ags.api.AmazonGames;
import com.amazon.ags.api.AmazonGamesClient;

import com.amazon.ags.api.overlay.PopUpLocation;

import com.amazon.ags.api.achievements.Achievement;
import com.amazon.ags.api.achievements.AchievementsClient;
import com.amazon.ags.api.achievements.AGAchievements;
import com.amazon.ags.api.achievements.GetAchievementResponse;
import com.amazon.ags.api.achievements.GetAchievementsResponse;
import com.amazon.ags.api.achievements.UpdateProgressResponse;

import com.amazon.ags.api.leaderboards.GetLeaderboardPercentilesResponse;
import com.amazon.ags.api.leaderboards.GetLeaderboardsResponse;
import com.amazon.ags.api.leaderboards.GetPlayerScoreResponse;
import com.amazon.ags.api.leaderboards.GetScoresResponse;
import com.amazon.ags.api.leaderboards.Leaderboard;
import com.amazon.ags.api.leaderboards.LeaderboardPercentileItem;
import com.amazon.ags.api.leaderboards.LeaderboardsClient;
import com.amazon.ags.api.leaderboards.Score;
import com.amazon.ags.api.leaderboards.SubmitScoreResponse;

import com.vungle.publisher.VunglePub;
import com.vungle.publisher.EventListener;

import android.widget.LinearLayout;
import android.widget.RelativeLayout;
/*import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;

import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;

*/
import com.google.android.gms.ads.*;

import android.view.Gravity;
import android.view.ViewGroup.LayoutParams;

import android.provider.Settings;
import android.view.View;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnClickListener;

import android.widget.Toast;

//import com.facebook.FacebookSdk;

public class MainActivity extends OxygineActivity
 implements
    GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener, IDownloaderClient
{

	private AdView mAdView;
	
	private boolean showAdMob = false;
	
	private RelativeLayout.LayoutParams layParamsAdMob;
	
	// Constants!!!!!!!!!!!!
	public static final String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAiu73uKMuGLi+w35PF/LtPuw4rrvU6WwZ/4Z+lo+UOg5K3XoYUC/0fKYCDJJIq1amwd5kkyigGbF5X5hrbsssH5nLILuQCMl7Ds1eNUPu7dlxC0BqYoGvirfxTDiuIuR/NDbsmjjOokFO/Ddp4F5KVCyViqNiWn+W5/cbfdJ0iJZ2wMhS7qz3cgv8Nk+VdAYKqISvS7oaO+CMgiAvMq1P/dfJDOV9DzRdmPDet56NkKDf+/paQusqsC4W3gNe7UrOyOwBVWMCfcuLMGtJ3ItN3CIPGBnqttoMqXCsfM3pZFM/mQzuF6fwQaqFewkZiz5rVoz/Ylqs1XG3ULzW5iczlwIDAQAB";
	public static final int m_ObbLength = 534634827;
	private static final boolean google = true;
	public static final Integer mainVersion = 12;// This is changing in the manifest and entry_point.cpp ( SDL_SetHint )
	public static final String dstMail = "divol13@gmail.com";
	public static final String gameName = "2048 feedback";
	//public static final String LEADERBOARD_ID = mSingleton.getString( R.string.leaderboard_table_of_scores );//"CgkI1YKYp7McEAIQAA";
	public static final String LEADERBOARD_ID_AMAZON = "crazy_scores_id";
	public static final int REQUEST_LEADERBOARD = 1234;
	
	final static VunglePub vunglePub = VunglePub.getInstance();
	
	//IMPORTANT!!! Flurry and billing data changing in the constants.h

	public static GoogleAnalytics analytics;
    public static Tracker tracker;	

	private final EventListener vungleListener = new EventListener(){

    @Deprecated
    @Override
    public void onVideoView(boolean isCompletedView, int watchedMillis, int videoDurationMillis) {
        // This method is deprecated and will be removed. Please do not use it.
        // Please use onAdEnd instead.  
    }

    @Override
    public void onAdStart() {
        // Called before playing an ad
    }

    @Override
    public void onAdEnd(boolean wasSuccessfulView, boolean wasCallToActionClicked) {
        // Called when the user leaves the ad and control is returned to your application        
        // if wasSuccessfulView is true, the user watched the ad and should be rewarded        
        // (if this was a rewarded ad).
        // if wasCallToActionClicked is true, the user clicked the call to action
        // button in the ad.
    }

    @Override
    public void onAdPlayableChanged(boolean isAdPlayable) {
        // Called when the playability state changes. if isAdPlayable is true, you can now        
        // play an ad.
        // If false, you cannot yet play an ad.
    }

    @Override
    public void onAdUnavailable(String reason) {
        // Called when VunglePub.playAd() was called, but no ad was available to play
    }

  };

	private static AmazonGamesClient agsClient;
	
	AmazonGamesCallback callback = new AmazonGamesCallback() {
        @Override
        public void onServiceNotReady(AmazonGamesStatus status) {
		Log.v ("SDL","onServiceNotReady " + status);
            //unable to use service
        }
        @Override
        public void onServiceReady(AmazonGamesClient amazonGamesClient) {
            agsClient = amazonGamesClient;
	    agsClient.setPopUpLocation(PopUpLocation.TOP_CENTER);
		Log.v ("SDL","onServiceReady");
            //ready to use GameCircle
        }
};

EnumSet<AmazonGamesFeature> myGameFeatures = EnumSet.of(
        AmazonGamesFeature.Achievements);
	
	

	private static IDownloaderService mRemoteService;
	private static IStub mDownloaderClientStub;
	//public static MainActivity mSingleton;
	private boolean mIntentInProgress = false;

	
	
	
	 private boolean mResolvingError = false;
	
	private static final int RC_SIGN_IN = 0;
	
	private static final int REQUEST_RESOLVE_ERROR = 1001;
	
	private static GoogleApiClient mGoogleApiClient;
	
	static private PackageManager g_PackageManager;
	
	private static final String TAG = "SDL";
	
	public void onStartGameInternal()
	{		
		runOnUiThread(new Runnable() {
            @Override
            public void run() 
			{	
				if( google )
				{
					Log.v(TAG, "try to mGoogleApiClient.connect");
					mGoogleApiClient.connect();
				}
				else
				{
					
				}
				 
				Chartboost.onStart(mSingleton);				
            }
        });
		
		
	}
	 public static void showLeaderBoard()
	{
	Log.v(TAG, "show leaderboard");
	if( google )
		{
				if (!mGoogleApiClient.isConnected()) {

					mGoogleApiClient.connect();
				}
				else				
				{
				String lboardId = mSingleton.getString( R.string.leaderboard_table_of_scores );
				
				Log.v(TAG, "leader board ID:" + lboardId);
				
				
	mSingleton.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(mGoogleApiClient, lboardId
        ), REQUEST_LEADERBOARD);
		}
		}
		else
		{
			if(agsClient == null) return;
				LeaderboardsClient lbClient = agsClient.getLeaderboardsClient();
				lbClient.showLeaderboardsOverlay();   
		}
	}	
	  
	  
	public static void showAchievements()
	{
		if( google )
		{
				if (!mGoogleApiClient.isConnected()) {

					mGoogleApiClient.connect();
				}
				else
				mSingleton.startActivityForResult(Games.Achievements.getAchievementsIntent(mGoogleApiClient),RC_SIGN_IN);
		}
		else
		{
			if (agsClient == null) return;
				AchievementsClient acClient = agsClient.getAchievementsClient();
				acClient.showAchievementsOverlay();   
		}			
	}
	
	private ChartboostDelegate delegate = new ChartboostDelegate() {

		@Override
		public boolean shouldRequestInterstitial(String location) {
			Log.i(TAG, "SHOULD REQUEST INTERSTITIAL '"+ (location != null ? location : "null"));		
			return true;
		}
	
		@Override
		public boolean shouldDisplayInterstitial(String location) {
			Log.i(TAG, "SHOULD DISPLAY INTERSTITIAL '"+ (location != null ? location : "null"));
			return true;
		}
	
		@Override
		public void didCacheInterstitial(String location) {
			Log.i(TAG, "DID CACHE INTERSTITIAL '"+ (location != null ? location : "null"));
		}

		@Override
		public void didFailToLoadInterstitial(String location, CBImpressionError error) {
			Log.i(TAG, "DID FAIL TO LOAD INTERSTITIAL '"+ (location != null ? location : "null")+ " Error: " + error.name());
			//Toast.makeText(getApplicationContext(), "INTERSTITIAL '"+location+"' REQUEST FAILED - " + error.name(), Toast.LENGTH_SHORT).show();
		}
	
		@Override
		public void didDismissInterstitial(String location) {
			Log.i(TAG, "DID DISMISS INTERSTITIAL: "+ (location != null ? location : "null"));
		}
	
		@Override
		public void didCloseInterstitial(String location) {
			Log.i(TAG, "DID CLOSE INTERSTITIAL: "+ (location != null ? location : "null"));
		}
	
		@Override
		public void didClickInterstitial(String location) {
			Log.i(TAG, "DID CLICK INTERSTITIAL: "+ (location != null ? location : "null"));
		}
	
		@Override
		public void didDisplayInterstitial(String location) {
			Log.i(TAG, "DID DISPLAY INTERSTITIAL: " +  (location != null ? location : "null"));
		}
	
		@Override
		public boolean shouldRequestMoreApps(String location) {
			Log.i(TAG, "SHOULD REQUEST MORE APPS: " +  (location != null ? location : "null"));
			return true;
		}
	
		@Override
		public boolean shouldDisplayMoreApps(String location) {
			Log.i(TAG, "SHOULD DISPLAY MORE APPS: " +  (location != null ? location : "null"));
			return true;
		}
	
		@Override
		public void didFailToLoadMoreApps(String location, CBImpressionError error) {
			Log.i(TAG, "DID FAIL TO LOAD MOREAPPS " +  (location != null ? location : "null")+ " Error: "+ error.name());
			//Toast.makeText(getApplicationContext(), "MORE APPS REQUEST FAILED - " + error.name(), Toast.LENGTH_SHORT).show();
		}
	
		@Override
		public void didCacheMoreApps(String location) {
			Log.i(TAG, "DID CACHE MORE APPS: " +  (location != null ? location : "null"));
		}
	
		@Override
		public void didDismissMoreApps(String location) {
			Log.i(TAG, "DID DISMISS MORE APPS " +  (location != null ? location : "null"));
		}
	
		@Override
		public void didCloseMoreApps(String location) {
			Log.i(TAG, "DID CLOSE MORE APPS: "+  (location != null ? location : "null"));
		}
	
		@Override
		public void didClickMoreApps(String location) {
			Log.i(TAG, "DID CLICK MORE APPS: "+  (location != null ? location : "null"));
		}
	
		@Override
		public void didDisplayMoreApps(String location) {
			Log.i(TAG, "DID DISPLAY MORE APPS: " +  (location != null ? location : "null"));
		}
	
		@Override
		public void didFailToRecordClick(String uri, CBClickError error) {
			Log.i(TAG, "DID FAILED TO RECORD CLICK " + (uri != null ? uri : "null") + ", error: " + error.name());
			//Toast.makeText(getApplicationContext(), "FAILED TO RECORD CLICK " + (uri != null ? uri : "null") + ", error: " + error.name(), Toast.LENGTH_SHORT).show();
		}
	
		@Override
		public boolean shouldDisplayRewardedVideo(String location) {
			Log.i(TAG, String.format("SHOULD DISPLAY REWARDED VIDEO: '%s'",  (location != null ? location : "null")));
			return true;
		}
	
		@Override
		public void didCacheRewardedVideo(String location) {
			Log.i(TAG, String.format("DID CACHE REWARDED VIDEO: '%s'",  (location != null ? location : "null")));
		}
	
		@Override
		public void didFailToLoadRewardedVideo(String location,
				CBImpressionError error) {
			Log.i(TAG, String.format("DID FAIL TO LOAD REWARDED VIDEO: '%s', Error:  %s",  (location != null ? location : "null"), error.name()));
			//Toast.makeText(getApplicationContext(), String.format("DID FAIL TO LOAD REWARDED VIDEO '%s' because %s", location, error.name()), Toast.LENGTH_SHORT).show();
		}
	
		@Override
		public void didDismissRewardedVideo(String location) {
			Log.i(TAG, String.format("DID DISMISS REWARDED VIDEO '%s'",  (location != null ? location : "null")));
		}
	
		@Override
		public void didCloseRewardedVideo(String location) {
			Log.i(TAG, String.format("DID CLOSE REWARDED VIDEO '%s'",  (location != null ? location : "null")));
		}
	
		@Override
		public void didClickRewardedVideo(String location) {
			Log.i(TAG, String.format("DID CLICK REWARDED VIDEO '%s'",  (location != null ? location : "null")));
		}
	
		@Override
		public void didCompleteRewardedVideo(String location, int reward) {
			Log.i(TAG, String.format("DID COMPLETE REWARDED VIDEO '%s' FOR REWARD %d",  (location != null ? location : "null"), reward));
		}
		
		@Override
		public void didDisplayRewardedVideo(String location) {
			Log.i(TAG, String.format("DID DISPLAY REWARDED VIDEO '%s' FOR REWARD", location));
		}
		
		@Override
		public void willDisplayVideo(String location) {
			Log.i(TAG, String.format("WILL DISPLAY VIDEO '%s", location));
		}
		
	};
	  
	public static void onStartGame()
	{	
		Log.v(TAG, "main activity onStart");
		(( MainActivity )mSingleton ).onStartGameInternal();
		
		
		
				
	}
	
	 private static class XAPKFile {
        public final boolean mIsMain;
        public final int mFileVersion;
        public final long mFileSize;

        XAPKFile(boolean isMain, int fileVersion, long fileSize) {
            mIsMain = isMain;
            mFileVersion = fileVersion;
            mFileSize = fileSize;
        }
    }
	
	public static int mOBBProgress = 0;	
	String mainHint;
	
	private static final XAPKFile[] xAPKS = {
		 
		 //mainHint = 1;//nativeGetHint("SDL_ANDROID_APK_EXPANSION_MAIN_FILE_VERSION");
		 //mainVersion = Integer.valueOf( mainHint );
		 
            new XAPKFile(
                    true, // true signifies a main file
                    mainVersion, // the version of the APK that the file was uploaded
                       // against
                     m_ObbLength  // the length of the file in bytes
            )
    };            
	
	 boolean expansionFilesDelivered() {
//	Log.v (TAG,"check expansionFilesDelivered");
        for (XAPKFile xf : xAPKS) {
            String fileName = Helpers.getExpansionAPKFileName(this, xf.mIsMain, xf.mFileVersion);
//		Log.v (TAG,fileName);
            if (!Helpers.doesFileExist(this, fileName, xf.mFileSize, false))
                return false;
        }
        return true;
    }
	
	public static int getPercentObb()
	{
		return mOBBProgress;
	}
	
	public static boolean isGoogle()
	{
		return google;
	}
	
	private void startVideoAdInternal()
	{
		runOnUiThread(new Runnable() {
            @Override
            public void run() 
			{	
				vunglePub.playAd();
            }
        });		
	}
	
	public static void startVideoAd()
	{
		(( MainActivity )mSingleton ).startVideoAdInternal();
	}	
	
	 @Override
	    public void onPause() {
		
		super.onPause();
		Chartboost.onPause(this);
		
		vunglePub.onPause();


		
	   if (agsClient != null) {
		agsClient.release();
		agsClient = null;
	    }

	    }
	
	 @Override
    public void onResume() 
    {
        Log.v(TAG, "onResume main()");
        super.onResume();  
		vunglePub.onResume();

	Chartboost.onResume(this);
	
	if( !google )	
	{
		Log.v(TAG, "AmazonGamesClient.init " + this + " " + callback + " " + myGameFeatures);
		AmazonGamesClient.initialize(this, callback, myGameFeatures);
	}
    }
	
	@Override
    protected void onCreate(Bundle savedInstanceState) {
		//mSingleton = this;
        PackageManager pm = getPackageManager();
		g_PackageManager = pm;
        String installer = pm.getInstallerPackageName(getPackageName());
        /*if (installer == "com.amazon.venezia")
            google = false;

	    google = false;*/
	   /* if( google )
		Log.v(TAG, "google now " );
	  */  
	    
		Log.v(TAG, "installer name: " + installer );
			
        //google = false;
		
		Log.v(TAG, "pre GoogleApiClient.Builder");
		
		analytics = GoogleAnalytics.getInstance(this);
		analytics.setLocalDispatchPeriod(1800);

		tracker = analytics.newTracker("UA-98474346-1"); 
		tracker.enableExceptionReporting(true);
		tracker.enableAdvertisingIdCollection(true);
		tracker.enableAutoActivityTracking(true);
	
		String name = "magic2048";
	
		/*Log.i(TAG, "screen name: " + name);
		tracker.setScreenName("Image~" + name);
		tracker.send(new HitBuilders.ScreenViewBuilder().build());*/		
	
		if( google )
		{
				mGoogleApiClient = new GoogleApiClient.Builder(this)
        .addConnectionCallbacks(this)
        .addOnConnectionFailedListener(this)
        .addApi(Plus.API).addScope(Plus.SCOPE_PLUS_LOGIN)
        .addApi(Games.API).addScope(Games.SCOPE_GAMES)
        .build();
						
		
		
			
			Log.v(TAG, "GoogleApiClient.Builder");
			
			//showPlaces();
			
			 if (mGoogleApiClient.isConnected()) {
				 
				Log.v(TAG, "mGoogleApiClient.isConnected");
       
			 }
		}
		else
		{
			
		}
		
    
	
		Chartboost.startWithAppId(this, "590ccccdf6cd4513e3384bc6", "d440479857715c0dafbf1cdd7a0bae2d5417c7ef");
		//Chartboost.startWithAppId(this, "57837d8204b0167c2de92f19", "d0ce6f6d85693576cfd9cd4594f66c31aebc6f0c");
		
		Chartboost.setAutoCacheAds(true);
		
		//Chartboost.cacheInterstitial(CBLocation.LOCATION_HOME_SCREEN);
		
		Chartboost.setLoggingLevel(Level.ALL);
		Chartboost.setDelegate(delegate);

		Chartboost.onCreate(this);
		
		Log.v(TAG, "start mv");
		
		Log.v(TAG, "mv = " +mainVersion );
		
		
		 
				
				
				if ( !true && google && !expansionFilesDelivered()) {

				
				mDownloaderClientStub = DownloaderClientMarshaller.CreateStub
                (this, SampleDownloaderService.class);
				
				Log.v(TAG, "mDownloaderClientStub created "  );
				
				if (null != mDownloaderClientStub) {
            mDownloaderClientStub.connect(this);
        }
		mOBBProgress = 0;        
                Log.v(TAG, "OBB does not exist - downloading");

            try {
                Intent launchIntent = MainActivity.this
                        .getIntent();
                Intent intentToLaunchThisActivityFromNotification = new Intent(
                        MainActivity
                        .this, MainActivity.this.getClass());
                intentToLaunchThisActivityFromNotification.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK |
                        Intent.FLAG_ACTIVITY_CLEAR_TOP);
                intentToLaunchThisActivityFromNotification.setAction(launchIntent.getAction());

                if (launchIntent.getCategories() != null) {
                    for (String category : launchIntent.getCategories()) {
                        intentToLaunchThisActivityFromNotification.addCategory(category);
                    }
                }

                // Build PendingIntent used to open this activity from
                // Notification
                PendingIntent pendingIntent = PendingIntent.getActivity(
                        MainActivity.this,
                        0, intentToLaunchThisActivityFromNotification,
                        PendingIntent.FLAG_UPDATE_CURRENT);
                // Request to start the download
                int startResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(this,
                        pendingIntent, SampleDownloaderService.class);

                if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED) {
                    // The DownloaderService has started downloading the files,
                    // show progress
                }

                  // starting the movie                                   
            } catch (NameNotFoundException e) {
                Log.v(TAG, "Cannot find own package! MAYDAY!");
                e.printStackTrace();
            }

	 }
	else 
	{
		mOBBProgress = 100;
		Log.v(TAG, "OBB is here");
	}
	
	
	if( !google )
		mOBBProgress = 100;
		
		mOBBProgress = 100;
	
		Log.v(TAG, "this step"  );

		if( google )		
			new FlurryAgent.Builder().withLogEnabled( false ).build( this, "XD2PYK3NXDJW2BW9HR7W" /* "6BPK8WK3T9RD9Q4NG45Y" */ ); /*XD2PYK3NXDJW2BW9HR7W - Release*/
		else
			new FlurryAgent.Builder().withLogEnabled( false ).build( this, "ZYMZR4J97XXGNH6HRKYN" );
		
/*        if (google)
            addObserver(new BillingGoogle(  ));
        else
            addObserver(new BillingAmazon());
	*/		

		//	mAdView = new AdView(_activity);
		
			
		//addObserver(new AdmobAdapter( this, "ca-app-pub-1114798689409363/5037924334", "" ));


        //!!!!!!!!!!!!!!!!!!! IT IS IMPORTANT !!!! super.onCreate should be AT THE END of method
		Log.v(TAG, "onCreate PRE");
        super.onCreate(savedInstanceState);
		Log.v(TAG, "onCreate POST");
		
		
		final String app_id = "588efbc05f5e338431000078";

      // initialize the Publisher SDK
		vunglePub.init(this, app_id);
		vunglePub.setEventListeners(vungleListener);

		//String android_id = Settings.Secure.getString(getContext().getContentResolver(),Settings.Secure.ANDROID_ID);
		
		
		Log.v(TAG, "ad test 1");
		mAdView = new AdView(this);
		Log.v(TAG, "ad test 2");
		mAdView.setAdUnitId("ca-app-pub-1114798689409363/5037924334");
		Log.v(TAG, "ad test 3");
		mAdView.setAdSize(AdSize.SMART_BANNER); //BANNER SIZE 
		Log.v(TAG, "ad test 4");
		//LinearLayout layout = (LinearLayout)findViewById(R.id.adView);
		Log.v(TAG, "ad test 5");
		
		layParamsAdMob = new RelativeLayout.LayoutParams(
		RelativeLayout.LayoutParams.WRAP_CONTENT, 
		RelativeLayout.LayoutParams.WRAP_CONTENT);

		layParamsAdMob.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		mLayout.addView(mAdView, layParamsAdMob);
		mAdView.setVisibility(View.GONE);
		//mAdView.setVisibility(View.GONE);
				
		Log.v(TAG, "ad test 6");
		
		Log.v(TAG, "ad test 7");
		MainActivity.this.runOnUiThread(new Runnable() {

						@Override
						public void run() {
							AdRequest adRequest = new AdRequest.Builder().build();
							Log.v(TAG, "Start ad loading!!!!!!!!!!!!!!!!!!!");
							mAdView.loadAd(adRequest);
							Log.v(TAG, "Finish ad loading!!!!!!!!!!!!!!!!!!!");
						}
					});
		
		Log.v(TAG, "ad test 8");
		
		mAdView.setAdListener(new AdListener() {

				@Override
				public void onAdLoaded() {
					super.onAdLoaded();

					MainActivity.this.runOnUiThread(new Runnable() {

						@Override
						public void run() {
							mLayout.requestLayout();							
						}
					});
				}

			});
		
    }
	
	public void rateAppInternal( final String title, final String msg, final String ok, final String cancel ) 
	{
		runOnUiThread(new Runnable() {
            @Override
            public void run() 
			{	
			if( google )
			{
				AlertDialog.Builder ad = new AlertDialog.Builder(MainActivity.this);
				ad.setTitle(title);  // title
				ad.setMessage(msg); // message																						
				ad.setPositiveButton( ok, new OnClickListener() {
					public void onClick(DialogInterface dialog, int arg1) {
						Log.v (TAG,"onClick b1");
						sharedFunc ("rate", "market://details?id=com.divol13.magic2048");
					}
				});
				ad.setNegativeButton(cancel, new OnClickListener() {
					public void onClick(DialogInterface dialog, int arg1) {
						Log.v (TAG,"onClick b2");
					}
				});
				ad.setCancelable(true);
				ad.setOnCancelListener(new OnCancelListener() {
					public void onCancel(DialogInterface dialog) {
						Log.v (TAG,"onClick out");
					}
				});
				
				ad.show();
			}
				
            }
        });
	}
	
	public static void rateApp( String title, String msg, String ok, String canel ) 
	{
		(( MainActivity )mSingleton ).rateAppInternal( title, msg, ok, canel );
	}
	
	@Override
	public void onBackPressed() {
	     // If an interstitial is on screen, close it. Otherwise continue as normal.
        /*if (Chartboost.onBackPressed())
            return;
        else
            super.onBackPressed();*/
			 super.onBackPressed();
	}
	
	
	@Override
    public void onServiceConnected(Messenger m) {
			Log.v (TAG,"onServiceConnected");
	if (mDownloaderClientStub != null)
	{
        	mRemoteService = DownloaderServiceMarshaller.CreateProxy(m);
        	mRemoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
	}
    }


    /**
     * The download state should trigger changes in the UI --- it may be useful
     * to show the state as being indeterminate at times. This sample can be
     * considered a guideline.
     */
    @Override
    public void onDownloadStateChanged(int newState) {
	Log.v (TAG,"onDownloadStateChanged: "+ newState);
        switch (newState) {
            case IDownloaderClient.STATE_IDLE:
                // STATE_IDLE means the service is listening, so it's
                // safe to start making calls via mRemoteService.
                break;
            case IDownloaderClient.STATE_CONNECTING:
            case IDownloaderClient.STATE_FETCHING_URL:
                break;
            case IDownloaderClient.STATE_DOWNLOADING:
                break;

            case IDownloaderClient.STATE_FAILED_CANCELED:
            case IDownloaderClient.STATE_FAILED:
            case IDownloaderClient.STATE_FAILED_FETCHING_URL:
            case IDownloaderClient.STATE_FAILED_UNLICENSED:
                break;
            case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
                break;

            case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
                break;
            case IDownloaderClient.STATE_PAUSED_ROAMING:
            case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
                break;
            case IDownloaderClient.STATE_COMPLETED:
			mOBBProgress = 100;
                return;
        }
    }

    /**
     * Sets the state of the various controls based on the progressinfo object
     * sent from the downloader service.
     */
    @Override
    public void onDownloadProgress(DownloadProgressInfo progress) {
        mOBBProgress = (int)(progress.mOverallProgress * 100 / progress.mOverallTotal);
		
	Log.v (TAG,"onDownloadProgress: "+ mOBBProgress);
    }
	
	 
	protected void onStopInternal ()
{        
        //Log.v(TAG, "Flurry end");
		Chartboost.onStop(this);
		
		 Chartboost.onDestroy(this);
		 vunglePub.clearEventListeners();

	      FlurryAgent.onEndSession (this);	
        //Log.v(TAG, "Flurry end finish");

		if( google )
		{
			if (mGoogleApiClient.isConnected()) 
			{
				mGoogleApiClient.disconnect();
			}
		}
		else
		{
			//if (agsClient != null) agsClient.shutdown();
		}
 
	
	if (null != mDownloaderClientStub) {
            mDownloaderClientStub.disconnect(this);
        }

       /* if (null != mDownloaderClientStub) {
            mDownloaderClientStub.disconnect(this);
        }*/

}

	public static void onStopGame()
	{
		Log.v(TAG, "main activity onStopGame");
		(( MainActivity )mSingleton ).onStopInternal();
	}
	
	 public static boolean FlurryEvent (String e1, int time, String e2, String e3)
	 {
	  try {

			if(!google)
				e1 = "amazon_"+e1;
		
		  PackageManager pm = g_PackageManager;
		  ApplicationInfo appInfo = pm.getApplicationInfo("com.divol13.magic2048", 0);
		  String appFile = appInfo.sourceDir;
		  long installed = new File(appFile).lastModified();
		  int mLastEventTime = time;
		  Map<String, String> aParams = new HashMap<String, String>();
		  Double t_log_cal = (24.0 * Math.log ((System.currentTimeMillis () - installed)/1000));
		  Double t_log_play = (24.0 * Math.log ((time + 1.0f)));
			
		  aParams.put("t_log_cal", String.valueOf(t_log_cal.intValue ()));
		  aParams.put("t_log_play", String.valueOf(t_log_play.intValue ()));
		  if (!e2.isEmpty ()) aParams.put("t_param_1", e2);
		  if (!e3.isEmpty ()) aParams.put("t_param_2", e3);


		  Log.d(TAG, "push event flurry");
		  FlurryAgent.logEvent(e1, aParams); 
					} catch (NameNotFoundException e) {
						
						Log.v(TAG, "Cannot find own package! MAYDAY!");
						e.printStackTrace();
					}
		  return true;

	 }
	 
	 
	 @Override
public void onConnectionFailed(ConnectionResult result) {
   
	if( !google )
		return;
   Log.v(TAG, "onConnectionFailed");
   
    if (mResolvingError) {
            // Already attempting to resolve an error.
            return;
        } else if (result.hasResolution()) {
            Log.v(TAG,result.toString());
            try {
                mResolvingError = true;
                result.startResolutionForResult(this, REQUEST_RESOLVE_ERROR);
            } catch (SendIntentException e) {
                // There was an error with the resolution intent. Try again.
				Log.v(TAG,"catch (SendIntentException e)");
                mGoogleApiClient.connect();
            }
        } else {
            // Show dialog using GooglePlayServicesUtil.getErrorDialog()
            //showErrorDialog(result.getErrorCode());
            mResolvingError = true;
        }
   
   /* if (!mIntentInProgress && result.hasResolution()) {
        try {
            mIntentInProgress = true;
            result.startResolutionForResult(this, 
                    RC_SIGN_IN);
        } catch (SendIntentException e) {
            
            mIntentInProgress = false;
            mGoogleApiClient.connect();
        }
    }
*/
}

 @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if( !google )
			return;
        if (requestCode == REQUEST_RESOLVE_ERROR) {
			Log.v(TAG, "requestCode == REQUEST_RESOLVE_ERROR");
            mResolvingError = false;
            if (resultCode == RESULT_OK) {
				Log.v(TAG, "resultCode == RESULT_OK");
                // Make sure the app is not already connected or attempting to connect
                if (!mGoogleApiClient.isConnecting() && !mGoogleApiClient.isConnected()) {
					Log.v(TAG, "resultCode == RESULT_OK connect");
                    mGoogleApiClient.connect();
                }
            }
        }
					
		super.onActivityResult( requestCode, resultCode, data );
    }

@Override
public void onConnectionSuspended(int i) {
	if( !google )
			return;
	Log.v(TAG, "onConnectionSuspended() called. Trying to reconnect.");
    mGoogleApiClient.connect();
}

@Override
public void onConnected(Bundle connectionHint) {
    // TODO Auto-generated method stub
	Log.v(TAG, "onConnected() called. Sign in successful!");

}

/*@Override
public void onDisconnected() {
    // TODO Auto-generated method stub

}*/
	 
	 /* public void showPlaces() {

    PendingResult<PlaceLikelihoodBuffer> result = Places.PlaceDetectionApi.getCurrentPlace(mGoogleApiClient, null);
    result.setResultCallback(new ResultCallback<PlaceLikelihoodBuffer>() {
        @Override
        public void onResult(PlaceLikelihoodBuffer likelyPlaces) {
            StringBuffer sb=new StringBuffer();
            for (PlaceLikelihood placeLikelihood : likelyPlaces) {
                String name, per;
                name = placeLikelihood.getPlace().getName().toString()+"      ";
                per = ""+(int) (placeLikelihood.getLikelihood() * 100)+"\n";
                sb.append(name+per);
            }
            content = sb.toString();
            mTextView.setText(content);
            likelyPlaces.release();
        }
    });
}
	 */
	 
	 public static boolean isAchiAllow()
	 {
		 if( google )
		 {
				if( mGoogleApiClient == null )
			return false;
			
			return mGoogleApiClient.isConnected();
		 }
		 else
		 {
			 if (agsClient == null) return false;
			 
			 return true;
			 
		 }		 		
	 }
	 
	 
	 public static void scheduleNotification(Notification notification, int delay) {

        Intent notificationIntent = new Intent(mSingleton, NotificationPublisher.class);
        notificationIntent.putExtra(NotificationPublisher.NOTIFICATION_ID, 1);
        notificationIntent.putExtra(NotificationPublisher.NOTIFICATION, notification);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(mSingleton, 0, notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        long futureInMillis = SystemClock.elapsedRealtime() + delay;
        AlarmManager alarmManager = (AlarmManager)mSingleton.getSystemService(Context.ALARM_SERVICE);
        alarmManager.set(AlarmManager.ELAPSED_REALTIME_WAKEUP, futureInMillis, pendingIntent);
    }

    public static Notification getNotification(String content) {
 
                Intent launchIntent = mSingleton
                        .getIntent();
                Intent intentToLaunchThisActivityFromNotification = new Intent(
                        mSingleton
                       , mSingleton.getClass());
                intentToLaunchThisActivityFromNotification.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP |
                            Intent.FLAG_ACTIVITY_SINGLE_TOP);
                intentToLaunchThisActivityFromNotification.setAction(launchIntent.getAction());

                if (launchIntent.getCategories() != null) {
                    for (String category : launchIntent.getCategories()) {
                        intentToLaunchThisActivityFromNotification.addCategory(category);
                    }
                }

                // Build PendingIntent used to open this activity from
                // Notification
                PendingIntent pendingIntent = PendingIntent.getActivity(
                        mSingleton,
                        0, intentToLaunchThisActivityFromNotification,
                        PendingIntent.FLAG_UPDATE_CURRENT);


        Notification.Builder builder = new Notification.Builder(mSingleton);
        builder.setContentTitle(mSingleton.getString (R.string.gameheader));
		
        builder.setContentText(content);
	builder.setContentIntent (pendingIntent);
        builder.setSmallIcon(R.drawable.ic_launcher);
        return builder.getNotification ();                     
    }

	
	public void googleAnalyticsFuncInternal( final String category, final String action, final String label )
	{
	
	runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tracker.send(new HitBuilders.EventBuilder()
			.setCategory(category)
			.setAction(action)
			.setLabel(label)
			.build());	
            }
        });	
	}
	
	public static boolean googleAnalyticsFunc(String category,String action, String label)
	{
		(( MainActivity )mSingleton ).googleAnalyticsFuncInternal( category, action, label );
		return true;
	}
	
	public void sharedFuncInternal( final String title, final String param)
	{
	
	runOnUiThread(new Runnable() {
            @Override
            public void run() 
			{
                if (title.equals ("notification"))
				{
					scheduleNotification(getNotification(/*mSingleton.getString (R.string.gametext) */ param), 1000 *60 * 60 * 23); 
					Log.v(TAG, "notification send: " + param);								
				} 
				else if (title.equals ("moregames"))
				{
					//Chartboost.showMoreApps(CBLocation.LOCATION_GAME_SCREEN);					
				}
				else if (title.equals ("show_ad"))
				{
					Chartboost.showInterstitial(CBLocation.LOCATION_HOME_SCREEN);					
				}
				else if (title.equals ("rate"))
				{
					mSingleton.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(param)));
				}
				else if (title.equals ("fb"))
				{
					mSingleton.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(param)));						
				}
            }
        });
	
	}	
	 
	public static boolean sharedFunc (String title, String param)
	{
		(( MainActivity )mSingleton ).sharedFuncInternal( title, param );
		return true;
	}
	 
	 
	public static boolean sendEmail()
	{		
		Intent i = new Intent(Intent.ACTION_SEND);
		i.setType("message/rfc822");
		i.putExtra(Intent.EXTRA_EMAIL  , new String[]{ dstMail });
		i.putExtra(Intent.EXTRA_SUBJECT, gameName );
		i.putExtra(Intent.EXTRA_TEXT   , 
	                  
        			"Model: " + android.os.Build.MODEL
			);
		try {
		    mSingleton.startActivity(Intent.createChooser(i, "Send e-mail..."));
		} catch (android.content.ActivityNotFoundException ex) {
		    Toast.makeText(mSingleton, "There are no email clients installed.", Toast.LENGTH_SHORT).show();
		}

		return true;
	}	
	
	public static void setScores(int value)
	{
		if( google )		
		{
			if (!mGoogleApiClient.isConnected()) {

				Log.v(TAG, "if (!mGoogleApiClient.isConnected())");
				//mGoogleApiClient.connect();
				return;
			}
			Games.Leaderboards.submitScore(mGoogleApiClient, mSingleton.getString( R.string.leaderboard_table_of_scores ), value);
		}
		else
		{
		LeaderboardsClient lbClient = agsClient.getLeaderboardsClient();
		AGResponseHandle<SubmitScoreResponse> handle = lbClient.submitScore(LEADERBOARD_ID_AMAZON, value);

		// Optional callback to receive notification of success/failure.
		handle.setCallback(new AGResponseCallback<SubmitScoreResponse>() {

			@Override
			public void onComplete(SubmitScoreResponse result) {
				if (result.isError()) {
					// Add optional error handling here.  Not strictly required
					// since retries and on-device request caching are automatic.
				} else {
					// Continue game flow.
				}
			}
		});
		}
		
	}
	
	private void setupAds() {
		// And this is the same, but done programmatically
		
	}
	
	public void showAdInternal()
	{
		showAdMob = true;
		Log.v(TAG, "showAdInternal 1" );
		
		
		runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mAdView.setVisibility(View.VISIBLE);	
            }
        });
		
		
	}
	
	public static void showAd()
	{
		(( MainActivity )mSingleton ).showAdInternal();		
	}
	
	public void rateNowInternal()
	{
		runOnUiThread(new Runnable() {
            @Override
            public void run() {
                (( MainActivity )mSingleton ).sharedFunc ("rate", "market://details?id=com.divol13.magic2048");
            }
        });		
	}
	
	public static void rateNow()
	{
		(( MainActivity )mSingleton ).rateNowInternal();				
	}
	
	public void hideAdInternal()
	{
		showAdMob = false;
		Log.v(TAG, "hideAdInternal 1" );
		//mLayout.removeView(mAdView); 
		//layParamsAdMob = null;
		runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mAdView.setVisibility(View.GONE);								
            }
        });
		
	}
	
	public static void hideAd()
	{
		(( MainActivity )mSingleton ).hideAdInternal();		
	}
	
	
	 
	 public static boolean setAmazonAchievement( String title, int value )
	 {
		 Log.v(TAG, "setAchievement" + " " + agsClient);
		if (agsClient == null) return false;
		AchievementsClient acClient = agsClient.getAchievementsClient();
		String t = title;
		float v = value;
		
		AGResponseHandle<UpdateProgressResponse> handle = acClient.updateProgress(t, v);
		 
		 return true;
	 }
	 
	 public static boolean setAchievement(String title, int value)
	{
		if( !google )
		{
			return setAmazonAchievement( title, value );
		}
		Log.v(TAG, "setAchievement start");
		//int v = R.string.id_keen_eye;
		if (!mGoogleApiClient.isConnected()) {

			Log.v(TAG, "if (!mGoogleApiClient.isConnected())");
			mGoogleApiClient.connect();
			return false;
		}
		
		Log.v(TAG, "setAchievement pre " + title);
	
		/*
		if (title.equals ("id_session_1"))
		{
			Games.Achievements.unlock(mGoogleApiClient,
				mSingleton.getString (R.string.id_session_1));

		}
		*/
		
		return true;
	}
}
