var map;
var infowindow;
var service;
var marker;
var zoom = {
    'country': 3,
    'administrative_area_level_1': 7,
    'locality': 10,
    'sublocality_level_1': 15,
    'sublocality_level_2': 15,
    'sublocality': 15,
    'neighborhood': 15,
    'street_number': 15
}
var countryRestrict = { 'country': 'all' };
var search;
var detail_addr;

function initMap() { // 구글 지도 초기화
    var location = {
        lat: -33.870775,
        lng: 151.199025
    };
    var center = {
        lat: location['lat']-15.0,
        lng: location['lng']
    };
    map = new google.maps.Map(document.getElementById('map'), {
            zoom: 3,
            center: center,
            styles: [
                {
                    elementType: 'geometry',
                    stylers: [{
                        color: '#242f3e'
                }]
            },
                {
                    elementType: 'labels.text.stroke',
                    stylers: [{
                        color: '#242f3e'
                }]
            },
                {
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#746855'
                }]
            },
                {
                    featureType: 'administrative.locality',
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#d59563'
                }]
            },
                {
                    featureType: 'poi',
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#d59563'
                }]
            },
                {
                    featureType: 'poi.park',
                    elementType: 'geometry',
                    stylers: [{
                        color: '#263c3f'
                }]
            },
                {
                    featureType: 'poi.park',
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#6b9a76'
                }]
            },
                {
                    featureType: 'road',
                    elementType: 'geometry',
                    stylers: [{
                        color: '#38414e'
                }]
            },
                {
                    featureType: 'road',
                    elementType: 'geometry.stroke',
                    stylers: [{
                        color: '#212a37'
                }]
            },
                {
                    featureType: 'road',
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#9ca5b3'
                }]
            },
                {
                    featureType: 'road.highway',
                    elementType: 'geometry',
                    stylers: [{
                        color: '#746855'
                }]
            },
                {
                    featureType: 'road.highway',
                    elementType: 'geometry.stroke',
                    stylers: [{
                        color: '#1f2835'
                }]
            },
                {
                    featureType: 'road.highway',
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#f3d19c'
                }]
            },
                {
                    featureType: 'transit',
                    elementType: 'geometry',
                    stylers: [{
                        color: '#2f3948'
                }]
            },
                {
                    featureType: 'transit.station',
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#d59563'
                }]
            },
                {
                    featureType: 'water',
                    elementType: 'geometry',
                    stylers: [{
                        color: '#17263c'
                }]
            },
                {
                    featureType: 'water',
                    elementType: 'labels.text.fill',
                    stylers: [{
                        color: '#515c6d'
                }]
            },
                {
                    featureType: 'water',
                    elementType: 'labels.text.stroke',
                    stylers: [{
                        color: '#17263c'
                }]
            }
        ],
        zoomControl: false,
        fullscreenControl: false,
        streetViewControl: false,
        mapTypeControl: false
    });
    
    infowindow = new google.maps.InfoWindow();
    service = new google.maps.places.PlacesService(map);
    initAutocomplete();
    
    var memo;
    if (pid) {
        service.getDetails({
            placeId: pid
        }, function (place, status) {
            if (status === google.maps.places.PlacesServiceStatus.OK) {
                var temp;
                for (var i = 0; i < place.address_components.length; i++) {
                    var addressType = place.address_components[i].types[0];
                    if (temp = zoom[addressType]) {
                        map.setZoom(temp);
                        break;
                    }
                }
                if (temp < 10) {
                    if (temp < 7) default_lat -= 15;
                    else default_lat -= 7;
                }
                map.panTo({lat: default_lat, lng: default_lng});
                createMarker(place.geometry.location, place.formatted_address);
            }
        });
    } else {
        memo = "Hi";
        createMarker(location, memo);
    }
}

function initAutocomplete() {
    /* main search autocomplete */
    search = new google.maps.places.Autocomplete(
        (document.getElementById('search')), {
            types: ['geocode']
        });

    /* upload address autocomplete */
    detail_addr = new google.maps.places.Autocomplete(
        (document.getElementById('detail-addr')), {
            types: ['(cities)'],
            componentRestrictions: countryRestrict
        });

    $("#nation").change(function () {
        var country = document.getElementById("nation").value;
        if (country == 0) {
            detail_addr.setComponentRestrictions([]);
        } else {
            detail_addr.setComponentRestrictions({
                'country': country
            });
        }
    });
}

function createMarker(pos, memo) {
    marker = new google.maps.Marker({
        position: pos,
        icon: "../../img/basic/marker.svg",
        map: map,
        animation: google.maps.Animation.DROP
    });
    marker.addListener('click', function () {
        if (marker.getAnimation() !== null) {
            marker.setAnimation(null);
        } else {
            marker.setAnimation(google.maps.Animation.BOUNCE);
        }
    });
    google.maps.event.addListener(marker, 'click', function () {
        infowindow.setContent('<div>' + memo + '</div>');
        infowindow.open(map, this);
    });
}
