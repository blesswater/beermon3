PROJECT=RefMonitor
WWW_LOCATION=/var/www/html


echo "cp -r $WWW_LOCATION/*.html ./$PROJECT"
cp -r $WWW_LOCATION/*.html ./$PROJECT
echo "cp -r $WWW_LOCATION/css/*.css ./$PROJECT/css"
cp -r $WWW_LOCATION/css/*.css ./$PROJECT/css
echo "cp -r $WWW_LOCATION/js/*.js ./$PROJECT/js"
cp -r $WWW_LOCATION/js/*.js ./$PROJECT/js
echo "cp -r $WWW_LOCATION/cgi/*.py ./$PROJECT/cgi/*.py"
cp -r $WWW_LOCATION/cgi/*.py ./$PROJECT/cgi/*.py

