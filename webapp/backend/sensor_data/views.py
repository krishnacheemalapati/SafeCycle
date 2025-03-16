from rest_framework import viewsets
from rest_framework.response import Response
from rest_framework.decorators import api_view
from .models import SensorReading
from .serializers import SensorReadingSerializer
from channels.layers import get_channel_layer
from asgiref.sync import async_to_sync

class SensorReadingViewSet(viewsets.ModelViewSet):
    queryset = SensorReading.objects.all()
    serializer_class = SensorReadingSerializer

    def create(self, request, *args, **kwargs):
        serializer = self.get_serializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        self.perform_create(serializer)
        
        # Broadcast the new reading to WebSocket clients
        channel_layer = get_channel_layer()
        async_to_sync(channel_layer.group_send)(
            "sensor_data",
            {
                "type": "sensor_update",
                "data": serializer.data
            }
        )
        
        return Response(serializer.data)

@api_view(['GET'])
def get_latest_readings(request):
    readings = SensorReading.objects.order_by('-timestamp')[:10]
    serializer = SensorReadingSerializer(readings, many=True)
    return Response(serializer.data) 