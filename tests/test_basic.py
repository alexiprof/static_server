# Start via `make test-debug` or `make test-release`
async def test_basic(service_client):
    response = await service_client.get('/')
    assert response.status == 404
    assert response.text == 'File not found'
