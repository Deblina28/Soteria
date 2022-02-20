import React from "react";
import logo from './logo.svg';
import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import {Container} from "react-bootstrap";
import floorplan1 from './images/floorplan1_large.png';
import Table from '@mui/material/Table';
import TableBody from '@mui/material/TableBody';
import TableCell from '@mui/material/TableCell';
import TableContainer from '@mui/material/TableContainer';
import TableHead from '@mui/material/TableHead';
import TableRow from '@mui/material/TableRow';
import Paper from '@mui/material/Paper';

function createData(loc, air_quality_index, particle_count, temperature, humidity) {
  return { loc, air_quality_index, particle_count, temperature, humidity };
}

const rows = [
  createData('Conference 1', 137463.474, 62.01, 29, 65.0),
  createData('Conference 2', 124836.887, 62.02, 27, 65.3),
  createData('Conference 3', 853374.143, 62.03, 28, 65.0),
  createData('Ballroom A', 182945.836, 62.04, 29, 65),
  createData('Kitchen', 199698.987, 62.05, 29, 65),
];

function App() {
const [data, setData] = React.useState(null);

//data from node server test
React.useEffect(() => {
  fetch("/api")
    .then((res) => res.json())
    .then((data) => setData(data.message));
}, []);

  return (
    <div className="App">
        Floorplan 1 
        <Container>
  <figure className="position-relative">
    <figcaption className="figcaption1"> Hazard </figcaption>
    <img src={floorplan1} alt="floorplan1"/>
  </figure>
</Container>
      <header className="App-header">
        <p>{!data ? "Loading..." : data}</p>
        <a>
         
        </a>
      </header>

      <TableContainer component={Paper}>
      <Table sx={{ minWidth: 500 }} aria-label="simple table">
        <TableHead>
          <TableRow>
            <TableCell>Location</TableCell>
            <TableCell align="right">Air Quality Index&nbsp;(%)</TableCell>
            <TableCell align="right">Particle Count&nbsp;(ppm)</TableCell>
            <TableCell align="right">Temperature&nbsp;(C)</TableCell>
            <TableCell align="right">Humidity&nbsp;(%)</TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {rows.map((row) => (
            <TableRow
              key={row.loc}
              sx={{ '&:last-child td, &:last-child th': { border: 0 } }}
            >
              <TableCell component="th" scope="row">
                {row.loc}
              </TableCell>
              <TableCell align="right">{row.air_quality_index}</TableCell>
              <TableCell align="right">{row.particle_count}</TableCell>
              <TableCell align="right">{row.temperature}</TableCell>
              <TableCell align="right">{row.humidity}</TableCell>
            </TableRow>
          ))}
        </TableBody>
      </Table>
    </TableContainer>




    </div>

    
  );
}

export default App;
