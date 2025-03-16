import Link from 'next/link';

export default function Home() {
  return (
    <main className="min-h-screen bg-gradient-to-b from-gray-900 to-gray-800 text-white">
      {/* Hero Section */}
      <section className="container mx-auto px-4 py-20">
        <div className="text-center">
          <h1 className="text-6xl font-bold mb-6">SafeCycle</h1>
          <p className="text-2xl text-gray-300 mb-8">
            Revolutionizing bicycle safety with real-time proximity detection
          </p>
          <Link 
            href="/dashboard" 
            className="bg-blue-600 hover:bg-blue-700 text-white font-bold py-3 px-8 rounded-full text-lg transition-colors"
          >
            View Live Dashboard
          </Link>
        </div>
      </section>

      {/* Features Section */}
      <section className="bg-gray-800 py-20">
        <div className="container mx-auto px-4">
          <h2 className="text-4xl font-bold text-center mb-12">Key Features</h2>
          <div className="grid md:grid-cols-3 gap-8">
            <div className="bg-gray-700 p-6 rounded-lg">
              <h3 className="text-xl font-bold mb-4">360° Detection</h3>
              <p className="text-gray-300">
                Three ultrasonic sensors provide comprehensive coverage around your bicycle
              </p>
            </div>
            <div className="bg-gray-700 p-6 rounded-lg">
              <h3 className="text-xl font-bold mb-4">Real-time Alerts</h3>
              <p className="text-gray-300">
                Visual and audible warnings when vehicles or objects get too close
              </p>
            </div>
            <div className="bg-gray-700 p-6 rounded-lg">
              <h3 className="text-xl font-bold mb-4">Data Analytics</h3>
              <p className="text-gray-300">
                Track and analyze your riding patterns and close-call incidents
              </p>
            </div>
          </div>
        </div>
      </section>

      {/* Future Plans Section */}
      <section className="container mx-auto px-4 py-20">
        <h2 className="text-4xl font-bold text-center mb-12">Future Plans</h2>
        <div className="grid md:grid-cols-2 gap-12">
          <div>
            <h3 className="text-2xl font-bold mb-4">Mobile App Integration</h3>
            <p className="text-gray-300">
              We're developing a mobile app that will provide route tracking, incident history,
              and real-time notifications directly to your smartphone.
            </p>
          </div>
          <div>
            <h3 className="text-2xl font-bold mb-4">Machine Learning Enhancement</h3>
            <p className="text-gray-300">
              Future updates will include AI-powered threat detection to better predict
              and warn about potential hazards based on historical data.
            </p>
          </div>
        </div>
      </section>
    </main>
  );
}
