from django.contrib import admin
from django.urls import path, include
from rest_framework.routers import DefaultRouter
from sensor_data.views import SensorReadingViewSet, get_latest_readings

# Create a router and register our viewset with it
router = DefaultRouter()
router.register(r'sensor-readings', SensorReadingViewSet)

urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/', include(router.urls)),
    path('api/latest/', get_latest_readings, name='latest-readings'),
]
